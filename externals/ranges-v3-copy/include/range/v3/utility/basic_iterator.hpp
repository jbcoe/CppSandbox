/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
#ifndef RANGES_V3_UTILITY_BASIC_ITERATOR_HPP
#define RANGES_V3_UTILITY_BASIC_ITERATOR_HPP

#include <utility>
#include <type_traits>
#include <meta/meta.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/range_access.hpp>
#include <range/v3/utility/concepts.hpp>
#include <range/v3/utility/nullptr_v.hpp>
#include <range/v3/utility/static_const.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/iterator_concepts.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \cond
        namespace detail
        {
            template<typename T>
            struct is_reference_to_const
              : std::false_type
            {};

            template<typename T>
            struct is_reference_to_const<T const&>
              : std::true_type
            {};

            template<typename T>
            struct is_reference_to_const<T const&&>
              : std::true_type
            {};

            //
            // True iff the user has explicitly disabled writability of this
            // iterator.  Pass the basic_iterator's Val parameter and its
            // nested ::reference type.
            //
            template<typename ValueParam, typename Ref>
            using iterator_writability_disabled =
                meta::fast_or<
                    std::is_const<Ref>,
                    is_reference_to_const<Ref>,
                    std::is_const<ValueParam>>;

            // operator[] must return a proxy in case iterator destruction invalidates
            // referents.
            // To see why, consider the following implementation of operator[]:
            //   reference operator[](difference_type n) const
            //   { return *(*this + n); }
            // The problem here is that operator[] would return a reference created from
            // a temporary iterator.
            template<typename Val>
            struct operator_brackets_value
            {
                using type = Val;
                template<typename I>
                static RANGES_CXX14_CONSTEXPR type apply(I const & i)
                {
                    return *i;
                }
            };

            template<typename I, typename Ref, typename CommonRef>
            struct operator_brackets_const_proxy
            {
                using type = struct proxy
                {
                private:
                    struct private_ {};
                    using value_t =
                        meta::if_<
                            meta::not_<Same<uncvref_t<Ref>, iterator_value_t<I>>>,
                            iterator_value_t<I>,
                            private_>;
                    I const it_;
                    RANGES_CXX14_CONSTEXPR
                    explicit proxy(I i)
                      : it_(std::move(i))
                    {}
                    friend struct operator_brackets_const_proxy;
                public:
                    RANGES_CXX14_CONSTEXPR
                    proxy(proxy const&) = default;
                    RANGES_CXX14_CONSTEXPR
                    proxy const & operator=(proxy &) const = delete;
                    RANGES_CXX14_CONSTEXPR operator Ref() const
                    {
                        return *it_;
                    }
                    operator CommonRef() const volatile
                    {
                        return *it_;
                    }
                    CONCEPT_REQUIRES(Convertible<Ref, value_t>())
                    RANGES_CXX14_CONSTEXPR operator value_t() const
                    {
                        return *it_;
                    }
                };
                static RANGES_CXX14_CONSTEXPR type apply(I i)
                {
                    return type{std::move(i)};
                }
            };

            template<typename I, typename Ref, typename CommonRef>
            struct operator_brackets_proxy
            {
                using type = struct proxy
                {
                private:
                    struct private_ {};
                    using value_t =
                        meta::if_<
                            meta::not_<Same<uncvref_t<Ref>, iterator_value_t<I>>>,
                            iterator_value_t<I>,
                            private_>;
                    I const it_;
                    RANGES_CXX14_CONSTEXPR
                    explicit proxy(I i)
                      : it_(std::move(i))
                    {}
                    friend struct operator_brackets_proxy;
                public:
                    RANGES_CXX14_CONSTEXPR operator Ref() const
                    {
                        return *it_;
                    }
                    operator CommonRef() const volatile
                    {
                        return *it_;
                    }
                    CONCEPT_REQUIRES(Convertible<Ref, value_t>())
                    RANGES_CXX14_CONSTEXPR operator value_t() const
                    {
                        return *it_;
                    }
                    RANGES_CXX14_CONSTEXPR
                    proxy (proxy const&) = default;
                    RANGES_CXX14_CONSTEXPR
                    proxy const & operator=(proxy&) const = delete;
                    // BUGBUG assign from common reference? from rvalue reference?
                    RANGES_CXX14_CONSTEXPR
                    proxy const & operator=(iterator_value_t<I> const & x) const
                    {
                        *it_ = x;
                        return *this;
                    }
                    RANGES_CXX14_CONSTEXPR
                    proxy const & operator=(iterator_value_t<I> && x) const
                    {
                        *it_ = std::move(x);
                        return *this;
                    }
                };
                static RANGES_CXX14_CONSTEXPR type apply(I i)
                {
                    return type{std::move(i)};
                }
            };

            template<typename I, typename Val, typename Ref, typename CommonRef>
            using operator_brackets_dispatch =
                meta::if_<
                    iterator_writability_disabled<Val, Ref>,
                    meta::if_<
                        std::is_pod<Val>,
                        operator_brackets_value<meta::_t<std::remove_const<Val>>>,
                        operator_brackets_const_proxy<I, Ref, CommonRef>>,
                    operator_brackets_proxy<I, Ref, CommonRef>>;

            // iterators whose dereference operators reference the same value
            // for all iterators into the same sequence (like many input
            // iterators) need help with their postfix ++: the referenced
            // value must be read and stored away before the increment occurs
            // so that *a++ yields the originally referenced element and not
            // the next one.
            template<typename I>
            struct postfix_increment_proxy
            {
                using value_type = iterator_value_t<I>;
            private:
                mutable value_type value_;
            public:
                RANGES_CXX14_CONSTEXPR
                postfix_increment_proxy() = default;
                RANGES_CXX14_CONSTEXPR
                explicit postfix_increment_proxy(I const& x)
                  : value_(*x)
                {}
                // Returning a mutable reference allows nonsense like
                // (*r++).mutate(), but it imposes fewer assumptions about the
                // behavior of the value_type.  In particular, recall that
                // (*r).mutate() is legal if operator* returns by value.
                RANGES_CXX14_CONSTEXPR value_type& operator*() const
                {
                    return value_;
                }
            };

            //
            // In general, we can't determine that such an iterator isn't
            // writable -- we also need to store a copy of the old iterator so
            // that it can be written into.
            template<typename I>
            struct writable_postfix_increment_proxy
            {
                using value_type = iterator_value_t<I>;
            private:
                mutable value_type value_;
                I it_;
            public:
                RANGES_CXX14_CONSTEXPR
                writable_postfix_increment_proxy() = default;
                RANGES_CXX14_CONSTEXPR
                explicit writable_postfix_increment_proxy(I x)
                  : value_(*x)
                  , it_(std::move(x))
                {}
                // Dereferencing must return a proxy so that both *r++ = o and
                // value_type(*r++) can work.  In this case, *r is the same as
                // *r++, and the conversion operator below is used to ensure
                // readability.
                RANGES_CXX14_CONSTEXPR
                writable_postfix_increment_proxy const & operator*() const
                {
                    return *this;
                }
                // So that iter_move(r++) moves the cached value out
                RANGES_CXX14_CONSTEXPR
                friend value_type && indirect_move(writable_postfix_increment_proxy const &ref)
                {
                    return std::move(ref.value_);
                }
                // Provides readability of *r++
                RANGES_CXX14_CONSTEXPR
                operator value_type &() const
                {
                    return value_;
                }
                // Provides writability of *r++
                template<typename T,
                    CONCEPT_REQUIRES_(Writable<I, T>())>
                RANGES_CXX14_CONSTEXPR
                void operator=(T const &x) const
                {
                    *it_ = x;
                }
                // This overload just in case only non-const objects are writable
                template<typename T,
                    CONCEPT_REQUIRES_(Writable<I, T>())>
                RANGES_CXX14_CONSTEXPR
                void operator=(T &x) const
                {
                    *it_ = x;
                }
                template<typename T,
                    CONCEPT_REQUIRES_(MoveWritable<I, T>())>
                RANGES_CXX14_CONSTEXPR
                void operator=(T &&x) const
                {
                    *it_ = std::move(x);
                }
                // Provides X(r++)
                RANGES_CXX14_CONSTEXPR
                operator I const &() const
                {
                    return it_;
                }
            };

            template<typename Ref, typename Val>
            using is_non_proxy_reference =
                std::is_convertible<
                    meta::_t<std::remove_reference<Ref>> const volatile *,
                    Val const volatile *>;

            // A metafunction to choose the result type of postfix ++
            //
            // Because the C++98 input iterator requirements say that *r++ has
            // type T (value_type), implementations of some standard
            // algorithms like lexicographical_compare may use constructions
            // like:
            //
            //          *r++ < *s++
            //
            // If *r++ returns a proxy (as required if r is writable but not
            // multipass), this sort of expression will fail unless the proxy
            // supports the operator<.  Since there are any number of such
            // operations, we're not going to try to support them.  Therefore,
            // even if r++ returns a proxy, *r++ will only return a proxy if
            // *r also returns a proxy.
            template<typename I, typename Val, typename Ref, typename Cat>
            using postfix_increment_result =
                meta::if_<
                    meta::and_<
                        // A proxy is only needed for readable iterators
                        std::is_convertible<Ref, Val const &>,
                        // No forward iterator can have values that disappear
                        // before positions can be re-visited
                        ranges::Derived<ranges::input_iterator_tag, Cat>>,
                    meta::if_<
                        is_non_proxy_reference<Ref, Val>,
                        postfix_increment_proxy<I>,
                        writable_postfix_increment_proxy<I>>,
                    I>;

            template<typename Cur, typename Enable = void>
            struct has_mixin
              : std::false_type
            {};

            template<typename Cur>
            struct has_mixin<Cur, meta::void_<typename Cur::mixin>>
              : std::true_type
            {};

            template<typename Cur>
            struct get_mixin
            {
                using type = typename Cur::mixin;
            };

            template<typename Cur>
            using mixin_base =
                meta::_t<meta::if_<has_mixin<Cur>, get_mixin<Cur>, meta::id<basic_mixin<Cur>>>>;

            auto iter_cat(range_access::InputCursorConcept*) ->
                ranges::input_iterator_tag;
            auto iter_cat(range_access::ForwardCursorConcept*) ->
                ranges::forward_iterator_tag;
            auto iter_cat(range_access::BidirectionalCursorConcept*) ->
                ranges::bidirectional_iterator_tag;
            auto iter_cat(range_access::RandomAccessCursorConcept*) ->
                ranges::random_access_iterator_tag;
        }
        /// \endcond

        /// \addtogroup group-utility Utility
        /// @{
        ///
        template<typename T>
        struct basic_mixin
        {
        private:
            T t_;
        public:
            constexpr basic_mixin() = default;
            RANGES_CXX14_CONSTEXPR
            basic_mixin(T t)
              : t_(std::move(t))
            {}
            RANGES_CXX14_CONSTEXPR
            T &get() noexcept
            {
                return t_;
            }
            /// \overload
            RANGES_CXX14_CONSTEXPR
            T const &get() const noexcept
            {
                return t_;
            }
        };

        template<typename S>
        struct basic_sentinel : detail::mixin_base<S>
        {
            // http://gcc.gnu.org/bugzilla/show_bug.cgi?id=60799
            #ifndef __GNUC__
        private:
            #endif
            friend range_access;
            template<typename Cur, typename OtherSentinel>
            friend struct basic_iterator;
            RANGES_CXX14_CONSTEXPR
            S &end() noexcept
            {
                return this->detail::mixin_base<S>::get();
            }
            RANGES_CXX14_CONSTEXPR
            S const &end() const noexcept
            {
                return this->detail::mixin_base<S>::get();
            }
        private:
            using detail::mixin_base<S>::get;
        public:
            RANGES_CXX14_CONSTEXPR basic_sentinel() = default;
            RANGES_CXX14_CONSTEXPR basic_sentinel(S end)
              : detail::mixin_base<S>(std::move(end))
            {}
            using detail::mixin_base<S>::mixin_base;
            constexpr bool operator==(basic_sentinel<S> const &) const
            {
                return true;
            }
            constexpr bool operator!=(basic_sentinel<S> const &) const
            {
                return false;
            }
        };

        template<typename Cur, typename S>
        struct basic_iterator
          : detail::mixin_base<Cur>
        {
        private:
            friend range_access;
            friend detail::mixin_base<Cur>;
            template<typename OtherCur, typename OtherS>
            friend struct basic_iterator;
            CONCEPT_ASSERT(detail::InputCursor<Cur>());
            using single_pass = range_access::single_pass_t<Cur>;
            using cursor_concept_t =
                meta::if_<
                    single_pass,
                    range_access::InputCursorConcept,
                    detail::cursor_concept_t<Cur>>;

            using detail::mixin_base<Cur>::get;
            RANGES_CXX14_CONSTEXPR Cur &pos() noexcept
            {
                return this->detail::mixin_base<Cur>::get();
            }
            RANGES_CXX14_CONSTEXPR Cur const &pos() const noexcept
            {
                return this->detail::mixin_base<Cur>::get();
            }

            // If Range models RangeFacade or if the cursor models
            // ForwardCursor, then positions must be equality comparable.
            // Otherwise, it's an InputCursor in an RangeFacade, so
            // all cursors are trivially equal.
            // BUGBUG this doesn't handle weak iterators.
            constexpr bool equal2_(basic_iterator const&,
                range_access::InputCursorConcept *) const
            {
                return true;
            }
            constexpr bool equal2_(basic_iterator const &that,
                range_access::ForwardCursorConcept *) const
            {
                return range_access::equal(pos(), that.pos());
            }
            constexpr bool equal_(basic_iterator const &that,
                std::false_type *) const
            {
                return basic_iterator::equal2_(that, _nullptr_v<cursor_concept_t>());
            }
            constexpr bool equal_(basic_iterator const &that,
                std::true_type *) const
            {
                return range_access::equal(pos(), that.pos());
            }
        public:
            using reference =
                decltype(range_access::current(std::declval<Cur const &>()));
            using value_type = range_access::cursor_value_t<Cur>;
            using iterator_category = decltype(detail::iter_cat(_nullptr_v<cursor_concept_t>()));
            using difference_type = range_access::cursor_difference_t<Cur>;
            using pointer = meta::_t<std::add_pointer<reference>>;
            using common_reference = common_reference_t<reference &&, value_type &>;
        private:
            using postfix_increment_result_t =
                detail::postfix_increment_result<
                    basic_iterator, value_type, reference, iterator_category>;
            using operator_brackets_dispatch_t =
                detail::operator_brackets_dispatch<basic_iterator, value_type, reference, common_reference>;
        public:
            constexpr basic_iterator() = default;
            RANGES_CXX14_CONSTEXPR basic_iterator(Cur pos)
              : detail::mixin_base<Cur>{std::move(pos)}
            {}
            template<typename OtherCur, typename OtherS,
                CONCEPT_REQUIRES_(Convertible<OtherCur, Cur>())>
            basic_iterator(basic_iterator<OtherCur, OtherS> that)
              : detail::mixin_base<Cur>{std::move(that.pos())}
            {}
            // Mix in any additional constructors defined and exported by the cursor
            using detail::mixin_base<Cur>::mixin_base;
            RANGES_CXX14_CONSTEXPR reference operator*() const
                noexcept(noexcept(range_access::current(std::declval<basic_iterator const &>().pos())))
            {
                return range_access::current(pos());
            }
            RANGES_CXX14_CONSTEXPR
            basic_iterator& operator++()
            {
                range_access::next(pos());
                return *this;
            }
            RANGES_CXX14_CONSTEXPR postfix_increment_result_t operator++(int)
            {
                postfix_increment_result_t tmp(*this);
                ++*this;
                return tmp;
            }
            // BUGBUG this doesn't handle weak iterators.
            constexpr friend bool operator==(basic_iterator const &left,
                basic_iterator const &right)
            {
                return left.equal_(right, _nullptr_v<std::is_same<Cur, S>>());
            }
            constexpr friend bool operator!=(basic_iterator const &left,
                basic_iterator const &right)
            {
                return !(left == right);
            }
            friend constexpr bool operator==(basic_iterator const &left,
                basic_sentinel<S> const &right)
            {
                return range_access::empty(left.pos(), right.end());
            }
            friend constexpr bool operator!=(basic_iterator const &left,
                basic_sentinel<S> const &right)
            {
                return !(left == right);
            }
            friend constexpr bool operator==(basic_sentinel<S> const & left,
                basic_iterator const &right)
            {
                return range_access::empty(right.pos(), left.end());
            }
            friend constexpr bool operator!=(basic_sentinel<S> const &left,
                basic_iterator const &right)
            {
                return !(left == right);
            }
            CONCEPT_REQUIRES(detail::BidirectionalCursor<Cur>())
            RANGES_CXX14_CONSTEXPR
            basic_iterator& operator--()
            {
                range_access::prev(pos());
                return *this;
            }
            CONCEPT_REQUIRES(detail::BidirectionalCursor<Cur>())
            RANGES_CXX14_CONSTEXPR
            basic_iterator operator--(int)
            {
                basic_iterator tmp(*this);
                --*this;
                return tmp;
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            RANGES_CXX14_CONSTEXPR
            basic_iterator& operator+=(difference_type n)
            {
                range_access::advance(pos(), n);
                return *this;
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            RANGES_CXX14_CONSTEXPR
            friend basic_iterator operator+(basic_iterator left, difference_type n)
            {
                left += n;
                return left;
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            RANGES_CXX14_CONSTEXPR
            friend basic_iterator operator+(difference_type n, basic_iterator right)
            {
                right += n;
                return right;
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            RANGES_CXX14_CONSTEXPR
            basic_iterator& operator-=(difference_type n)
            {
                range_access::advance(pos(), -n);
                return *this;
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            RANGES_CXX14_CONSTEXPR
            friend basic_iterator operator-(basic_iterator left, difference_type n)
            {
                left -= n;
                return left;
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            RANGES_CXX14_CONSTEXPR
            friend difference_type operator-(basic_iterator const &left,
                basic_iterator const &right)
            {
                return range_access::distance_to(right.pos(), left.pos());
            }
            // symmetric comparisons
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            RANGES_CXX14_CONSTEXPR
            friend bool operator<(basic_iterator const &left, basic_iterator const &right)
            {
                return 0 < (right - left);
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            RANGES_CXX14_CONSTEXPR
            friend bool operator<=(basic_iterator const &left, basic_iterator const &right)
            {
                return 0 <= (right - left);
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            RANGES_CXX14_CONSTEXPR
            friend bool operator>(basic_iterator const &left, basic_iterator const &right)
            {
                return (right - left) < 0;
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            RANGES_CXX14_CONSTEXPR
            friend bool operator>=(basic_iterator const &left, basic_iterator const &right)
            {
                return (right - left) <= 0;
            }
            // asymmetric comparisons
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            friend constexpr bool operator<(basic_iterator const &left,
                basic_sentinel<S> const &right)
            {
                return !range_access::empty(left.pos(), right.end());
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            friend constexpr bool operator<=(basic_iterator const &left,
                basic_sentinel<S> const &right)
            {
                return true;
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            friend constexpr bool operator>(basic_iterator const &left,
                basic_sentinel<S> const &right)
            {
                return false;
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            friend constexpr bool operator>=(basic_iterator const &left,
                basic_sentinel<S> const &right)
            {
                return range_access::empty(left.pos(), right.end());
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            friend constexpr bool operator<(basic_sentinel<S> const &left,
                basic_iterator const &right)
            {
                return false;
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            friend constexpr bool operator<=(basic_sentinel<S> const &left,
                basic_iterator const &right)
            {
                return range_access::empty(right.pos(), left.end());
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            friend constexpr bool operator>(basic_sentinel<S> const &left,
                basic_iterator const &right)
            {
                return !range_access::empty(right.pos(), left.end());
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            friend constexpr bool operator>=(basic_sentinel<S> const &left,
                basic_iterator const &right)
            {
                return true;
            }
            CONCEPT_REQUIRES(detail::RandomAccessCursor<Cur>())
            RANGES_CXX14_CONSTEXPR
            typename operator_brackets_dispatch_t::type
            operator[](difference_type n) const
            {
                return operator_brackets_dispatch_t::apply(*this + n);
            }
        };

        /// Get a cursor from a basic_iterator
        struct get_cursor_fn
        {
            template<typename Cur, typename Sent>
            RANGES_CXX14_CONSTEXPR
            Cur &operator()(basic_iterator<Cur, Sent> &it) const noexcept
            {
                detail::mixin_base<Cur> &mix = it;
                return mix.get();
            }
            template<typename Cur, typename Sent>
            RANGES_CXX14_CONSTEXPR
            Cur const &operator()(basic_iterator<Cur, Sent> const &it) const noexcept
            {
                detail::mixin_base<Cur> const &mix = it;
                return mix.get();
            }
            template<typename Cur, typename Sent>
            RANGES_CXX14_CONSTEXPR
            Cur operator()(basic_iterator<Cur, Sent> &&it) const
                noexcept(std::is_nothrow_copy_constructible<Cur>::value)
            {
                detail::mixin_base<Cur> &mix = it;
                return std::move(mix.get());
            }
        };

        /// \sa `get_cursor_fn`
        /// \ingroup group-utility
        namespace
        {
            constexpr auto &&get_cursor = static_const<get_cursor_fn>::value;
        }
        /// @}

        /// \cond
        // This is so that writable postfix proxy objects satisfy Readability
        template<typename T, typename I, typename Qual1, typename Qual2>
        struct basic_common_reference<T, detail::writable_postfix_increment_proxy<I>, Qual1, Qual2>
          : basic_common_reference<T, iterator_value_t<I>, Qual1, meta::quote_trait<std::add_lvalue_reference>>
        {};

        template<typename I, typename T, typename Qual1, typename Qual2>
        struct basic_common_reference<detail::writable_postfix_increment_proxy<I>, T, Qual1, Qual2>
          : basic_common_reference<iterator_value_t<I>, T, meta::quote_trait<std::add_lvalue_reference>, Qual2>
        {};
        /// \endcond
    }
}

/// \cond
namespace std
{
    template<typename Cur, typename S>
    struct iterator_traits< ::ranges::basic_iterator<Cur, S>>
    {
    private:
        using iterator = ::ranges::basic_iterator<Cur, S>;
    public:
        using difference_type = typename iterator::difference_type;
        using value_type = typename iterator::value_type;
        using reference = typename iterator::reference;
        using iterator_category =
            ::meta::_t<
                ::ranges::detail::downgrade_iterator_category<
                    typename iterator::iterator_category,
                    reference>>;
        using pointer = typename iterator::pointer;
    };
}
/// \endcond

#endif
