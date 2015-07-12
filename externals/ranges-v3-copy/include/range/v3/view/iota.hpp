/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2013-2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

#ifndef RANGES_V3_VIEW_IOTA_HPP
#define RANGES_V3_VIEW_IOTA_HPP

#include <climits>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <meta/meta.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/view_facade.hpp>
#include <range/v3/utility/concepts.hpp>
#include <range/v3/utility/static_const.hpp>
#include <range/v3/view/take_exactly.hpp>
#include <range/v3/view/delimit.hpp>

#ifndef RANGES_SUPPRESS_IOTA_WARNING
#ifdef __clang__
#define RANGES_IOTA_WARNING_MESSAGE(MSG) __attribute__((__deprecated__("(Actually it's not deprecated, this is just an informational warning.) " MSG)))
#elif defined(__GNUC__)
#define RANGES_IOTA_WARNING_MESSAGE(MSG) __attribute__((__warning__(MSG)))
#else
#define RANGES_IOTA_WARNING_MESSAGE(MSG) /**/
#endif
#else
#define RANGES_IOTA_WARNING_MESSAGE(MSG) /**/
#endif

namespace ranges
{
    inline namespace v3
    {
        namespace concepts
        {
            struct BidirectionalIncrementable
              : refines<Incrementable>
            {
                template<typename T>
                auto requires_(T&& t) -> decltype(
                    concepts::valid_expr(
                        concepts::has_type<T &>(--t),
                        concepts::has_type<T>(t--)
                    ));
            };

            struct RandomAccessIncrementable
              : refines<BidirectionalIncrementable>
            {
                template<typename T>
                auto requires_(T&& t) -> decltype(
                    concepts::valid_expr(
                        concepts::model_of<Integral>(t - t),
                        concepts::has_type<T &>(t += (t - t)),
                        concepts::has_type<T &>(t -= (t - t)),
                        concepts::convertible_to<T>(t - (t - t)),
                        concepts::convertible_to<T>(t + (t - t)),
                        concepts::convertible_to<T>((t - t) + t)
                    ));
            };
        }

        template<typename T>
        using BidirectionalIncrementable = concepts::models<concepts::BidirectionalIncrementable, T>;

        template<typename T>
        using RandomAccessIncrementable = concepts::models<concepts::RandomAccessIncrementable, T>;

        template<typename T>
        using incrementable_concept =
            concepts::most_refined<
                meta::list<
                    concepts::RandomAccessIncrementable,
                    concepts::BidirectionalIncrementable,
                    concepts::Incrementable,
                    concepts::WeaklyIncrementable>, T>;

        template<typename T>
        using incrementable_concept_t = meta::_t<incrementable_concept<T>>;

        /// \cond
        namespace detail
        {
            template<typename Val, typename Iota = incrementable_concept_t<Val>,
                bool IsIntegral = std::is_integral<Val>::value>
            struct iota_difference_
              : ranges::difference_type<Val>
            {};

            template<typename Val>
            struct iota_difference_<Val, concepts::RandomAccessIncrementable, true>
            {
            private:
                using difference_t = decltype(std::declval<Val>() - std::declval<Val>());
                static constexpr std::size_t bits = sizeof(difference_t) * CHAR_BIT;
            public:
                using type =
                    meta::if_<
                        meta::not_<std::is_same<Val, difference_t>>,
                        meta::_t<std::make_signed<difference_t>>,
                        meta::if_c<
                            (bits < 8),
                            std::int_fast8_t,
                            meta::if_c<
                                (bits < 16),
                                std::int_fast16_t,
                                meta::if_c<
                                    (bits < 32),
                                    std::int_fast32_t,
                                    std::int_fast64_t> > > >;
            };

            template<typename Val>
            struct iota_difference
              : iota_difference_<Val>
            {};

            template<typename Val>
            using iota_difference_t = meta::_t<iota_difference<Val>>;

            template<typename Val, CONCEPT_REQUIRES_(!Integral<Val>())>
            iota_difference_t<Val> iota_minus(Val const &v0, Val const &v1)
            {
                return v0 - v1;
            }

            template<typename Val, CONCEPT_REQUIRES_(SignedIntegral<Val>())>
            iota_difference_t<Val> iota_minus(Val const &v0, Val const &v1)
            {
                using D = iota_difference_t<Val>;
                return (D) v0 - (D) v1;
            }

            template<typename Val, CONCEPT_REQUIRES_(UnsignedIntegral<Val>())>
            iota_difference_t<Val> iota_minus(Val const &v0, Val const &v1)
            {
                using D = iota_difference_t<Val>;
                return (D) (v0 - v1);
            }
        }
        /// \endcond

        /// \addtogroup group-views
        /// @{

        /// An iota view in a closed range with non-random access iota value type
        template<typename From, typename To /* = From */>
        struct closed_iota_view
          : view_facade<closed_iota_view<From, To>, finite>
        {
        private:
            friend range_access;
            using difference_type_ = detail::iota_difference_t<From>;

            From from_;
            To to_;
            bool done_ = false;

            From current() const
            {
                return from_;
            }
            void next()
            {
                if(from_ == to_)
                    done_ = true;
                else
                    ++from_;
            }
            bool done() const
            {
                return done_;
            }
            CONCEPT_REQUIRES(Incrementable<From>())
            bool equal(closed_iota_view const &that) const
            {
                return that.from_ == from_;
            }
            CONCEPT_REQUIRES(BidirectionalIncrementable<From>())
            void prev()
            {
                --from_;
            }
            CONCEPT_REQUIRES(RandomAccessIncrementable<From>())
            void advance(difference_type_ n)
            {
                RANGES_ASSERT(detail::iota_minus(to_, from_) >= n);
                from_ += n;
            }
            CONCEPT_REQUIRES(RandomAccessIncrementable<From>())
            difference_type_ distance_to(closed_iota_view const &that) const
            {
                return detail::iota_minus(that.from_, from_);
            }
        public:
            closed_iota_view() = default;
            constexpr closed_iota_view(From from, To to)
              : from_(detail::move(from)), to_(detail::move(to))
            {}
        };

        template<typename From, typename To /* = void*/>
        struct iota_view
          : view_facade<iota_view<From, To>, finite>
        {
        private:
            friend range_access;
            using difference_type_ = detail::iota_difference_t<From>;

            From from_;
            To to_;

            From current() const
            {
                return from_;
            }
            void next()
            {
                ++from_;
            }
            bool done() const
            {
                return from_ == to_;
            }
            CONCEPT_REQUIRES(Incrementable<From>())
            bool equal(iota_view const &that) const
            {
                return that.from_ == from_;
            }
            CONCEPT_REQUIRES(BidirectionalIncrementable<From>())
            void prev()
            {
                --from_;
            }
            CONCEPT_REQUIRES(RandomAccessIncrementable<From>())
            void advance(difference_type_ n)
            {
                RANGES_ASSERT(detail::iota_minus(to_, from_) >= n);
                from_ += n;
            }
            CONCEPT_REQUIRES(RandomAccessIncrementable<From>())
            difference_type_ distance_to(iota_view const &that) const
            {
                return detail::iota_minus(that.from_, from_);
            }
        public:
            iota_view() = default;
            constexpr iota_view(From from, To to)
              : from_(detail::move(from)), to_(detail::move(to))
            {}
        };

        template<typename From>
        struct iota_view<From, void>
          : view_facade<iota_view<From, void>, infinite>
        {
        private:
            using incrementable_concept_t = ranges::incrementable_concept<From>;
            friend range_access;
            using difference_type_ = detail::iota_difference_t<From>;

            From value_;

            From current() const
            {
                return value_;
            }
            void next()
            {
                ++value_;
            }
            constexpr bool done() const
            {
                return false;
            }
            CONCEPT_REQUIRES(Incrementable<From>())
            bool equal(iota_view const &that) const
            {
                return that.value_ == value_;
            }
            CONCEPT_REQUIRES(BidirectionalIncrementable<From>())
            void prev()
            {
                --value_;
            }
            CONCEPT_REQUIRES(RandomAccessIncrementable<From>())
            void advance(difference_type_ n)
            {
                value_ += n;
            }
            CONCEPT_REQUIRES(RandomAccessIncrementable<From>())
            difference_type_ distance_to(iota_view const &that) const
            {
                return detail::iota_minus(that.value_, value_);
            }
        public:
            iota_view() = default;
            constexpr explicit iota_view(From value)
              : value_(detail::move(value))
            {}
        };

        namespace view
        {
            struct iota_fn
            {
            private:
                template<typename From>
                static take_exactly_view<iota_view<From>>
                impl(From from, From to, concepts::RandomAccessIncrementable *)
                {
                    return {iota_view<From>{std::move(from)}, detail::iota_minus(to, from)};
                }
                template<typename From, typename To>
                static iota_view<From, To>
                impl(From from, To to, concepts::WeaklyIncrementable *)
                {
                    return {std::move(from), std::move(to)};
                }
            public:
                template<typename From,
                    CONCEPT_REQUIRES_(WeaklyIncrementable<From>())>
                iota_view<From> operator()(From value) const
                {
                    return iota_view<From>{std::move(value)};
                }
                template<typename From, typename To>
                meta::if_c<
                    WeaklyIncrementable<From>() && EqualityComparable<From, To>(),
                    meta::if_<
                        meta::and_<RandomAccessIncrementable<From>, Same<From, To>>,
                        take_exactly_view<iota_view<From>>,
                        iota_view<From, To>>>
                operator()(From from, To to) const
                    RANGES_IOTA_WARNING_MESSAGE("The meaning of view::iota(x,y) has changed! It is no "
                        "longer a closed sequence that includes 'y'. It is a half-open sequence that "
                        "includes 'x' but excludes 'y'. Please change your code accordingly. You can "
                        "change the upper bound or use view::closed_iota. (Define "
                        "RANGES_SUPPRESS_IOTA_WARNING to turn off this warning.)");

            #ifndef RANGES_DOXYGEN_INVOKED
                template<typename From,
                    CONCEPT_REQUIRES_(!WeaklyIncrementable<From>())>
                void operator()(From) const
                {
                    CONCEPT_ASSERT_MSG(WeaklyIncrementable<From>(),
                        "The object passed to view::iota must model the WeaklyIncrementable concept; "
                        "that is, it must have pre- and post-increment operators and it must have a "
                        " difference_type");
                }
                template<typename From, typename To,
                    CONCEPT_REQUIRES_(!(WeaklyIncrementable<From>() && EqualityComparable<From, To>()))>
                void operator()(From, To) const
                {
                    CONCEPT_ASSERT_MSG(WeaklyIncrementable<From>(),
                        "The object passed to view::iota must model the WeaklyIncrementable concept; "
                        "that is, it must have pre- and post-increment operators and it must have a "
                        " difference_type");
                    CONCEPT_ASSERT_MSG(EqualityComparable<From, To>(),
                        "The two arguments passed to view::iota must be EqualityComparable with == and !=");
                }
            #endif
            };

            template<typename From, typename To>
            meta::if_c<
                WeaklyIncrementable<From>() && EqualityComparable<From, To>(),
                meta::if_<
                    meta::and_<RandomAccessIncrementable<From>, Same<From, To>>,
                    take_exactly_view<iota_view<From>>,
                    iota_view<From, To>>>
            iota_fn::operator()(From from, To to) const
            {
                return iota_fn::impl(std::move(from), std::move(to), incrementable_concept<From>{});
            }

            struct closed_iota_fn
            {
            private:
                template<typename From>
                static take_exactly_view<iota_view<From>>
                impl(From from, From to, concepts::RandomAccessIncrementable *)
                {
                    return {iota_view<From>{std::move(from)}, detail::iota_minus(to, from) + 1};
                }
                template<typename From, typename To>
                static closed_iota_view<From, To>
                impl(From from, To to, concepts::WeaklyIncrementable *)
                {
                    return {std::move(from), std::move(to)};
                }
            public:
                template<typename From, typename To,
                    CONCEPT_REQUIRES_(WeaklyIncrementable<From>() && EqualityComparable<From, To>())>
                meta::if_<
                    meta::and_<RandomAccessIncrementable<From>, Same<From, To>>,
                    take_exactly_view<iota_view<From>>,
                    closed_iota_view<From, To>>
                operator()(From from, To to) const
                {
                    return closed_iota_fn::impl(std::move(from), std::move(to), incrementable_concept<From>{});
                }
            #ifndef RANGES_DOXYGEN_INVOKED
                template<typename From, typename To,
                    CONCEPT_REQUIRES_(!(WeaklyIncrementable<From>() && EqualityComparable<From, To>()))>
                void operator()(From, To) const
                {
                    CONCEPT_ASSERT_MSG(WeaklyIncrementable<From>(),
                        "The object passed to view::closed_iota must model the WeaklyIncrementable concept; "
                        "that is, it must have pre- and post-increment operators and it must have a "
                        " difference_type");
                    CONCEPT_ASSERT_MSG(EqualityComparable<From, To>(),
                        "The two arguments passed to view::closed_iota must be EqualityComparable with == and !=");
                }
            #endif
            };

            /// \relates iota_fn
            /// \ingroup group-views
            namespace
            {
                constexpr auto&& iota = static_const<iota_fn>::value;
            }

            /// \relates closed_iota_fn
            /// \ingroup group-views
            namespace
            {
                constexpr auto&& closed_iota = static_const<closed_iota_fn>::value;
            }

            struct ints_fn
              : iota_view<int>
            {
                ints_fn() = default;

                template<typename Val,
                    CONCEPT_REQUIRES_(Integral<Val>())>
                iota_view<Val> operator()(Val value) const
                {
                    return iota_view<Val>{value};
                }
                template<typename Val>
                meta::if_c<
                    (bool)Integral<Val>(),
                    take_exactly_view<iota_view<Val> > >
                operator()(Val from, Val to) const
                    RANGES_IOTA_WARNING_MESSAGE("The meaning of view::ints(x,y) has changed! It is no "
                        "longer a closed sequence that includes 'y'. It is a half-open sequence that "
                        "includes 'x' but excludes 'y'. Please change your code accordingly. You can "
                        "change the upper bound or use view::closed_ints. (Define "
                        "RANGES_SUPPRESS_IOTA_WARNING to turn off this warning.)");

            #ifndef RANGES_DOXYGEN_INVOKED
                template<typename Val,
                    CONCEPT_REQUIRES_(!Integral<Val>())>
                void operator()(Val) const
                {
                    CONCEPT_ASSERT_MSG(Integral<Val>(),
                        "The object passed to view::ints must be Integral");
                }
                template<typename Val,
                    CONCEPT_REQUIRES_(!Integral<Val>())>
                void operator()(Val, Val) const
                {
                    CONCEPT_ASSERT_MSG(Integral<Val>(),
                        "The object passed to view::ints must be Integral");
                }
            #endif
            };

            template<typename Val>
            meta::if_c<
                (bool)Integral<Val>(),
                take_exactly_view<iota_view<Val>>>
            ints_fn::operator()(Val from, Val to) const
            {
                return {iota_view<Val>{from}, detail::iota_minus(to, from)};
            }

            struct closed_ints_fn
            {
                template<typename Val,
                    CONCEPT_REQUIRES_(Integral<Val>())>
                take_exactly_view<iota_view<Val>> operator()(Val from, Val to) const
                {
                    return {iota_view<Val>{from}, detail::iota_minus(to, from) + 1};
                }
            #ifndef RANGES_DOXYGEN_INVOKED
                template<typename Val,
                    CONCEPT_REQUIRES_(!Integral<Val>())>
                void operator()(Val, Val) const
                {
                    CONCEPT_ASSERT_MSG(Integral<Val>(),
                        "The object passed to view::closed_ints must be Integral");
                }
            #endif
            };

            /// \relates ints_fn
            /// \ingroup group-views
            namespace
            {
                constexpr auto&& ints = static_const<ints_fn>::value;
            }

            /// \relates closed_ints_fn
            /// \ingroup group-views
            namespace
            {
                constexpr auto&& closed_ints = static_const<closed_ints_fn>::value;
            }
        }
        /// @}
    }
}

#endif
