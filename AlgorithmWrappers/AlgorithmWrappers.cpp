#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>
#include <vector>

namespace rng
{
  template <typename R, typename F>
  bool any_of(const R& r, F&& f)
  {
    using std::begin;
    using std::end;
    return std::any_of(begin(r), end(r), std::forward<F>(f));
  }
  
  template <typename R, typename F>
  bool all_of(const R& r, F&& f)
  {
    using std::begin;
    using std::end;
    return std::all_of(begin(r), end(r), std::forward<F>(f));
  }
  
  template <typename R, typename F>
  bool none_of(const R& r, F&& f)
  {
    using std::begin;
    using std::end;
    return std::none_of(begin(r), end(r), std::forward<F>(f));
  }
  
  template <typename R, typename F>
  size_t count_if(const R& r, F&& f)
  {
    using std::begin;
    using std::end;
    return std::count_if(begin(r), end(r), std::forward<F>(f));
  }

  template <typename R, typename F>
  auto map(const R& r, F&& f)
  {
    using std::begin;
    using std::end;
    using result_type = decltype(f(r.front()));
    std::vector<result_type> results(r.size());
    std::transform(begin(r), end(r), results.begin(), std::forward<F>(f));
    return results;
  }

  template <typename R, typename F,
            typename V = decltype(*std::declval<const R&>().begin())>
  auto reduce(const R& r, F&& f, V v = V{})
  {
    using std::begin;
    using std::end;
    return std::accumulate(begin(r), end(r), std::move(v), std::forward<F>(f));
  }
} // end namespace rng

template <typename F>
class move_only_t
{
  F f_;

public:
  template <typename... Ts>
  auto operator()(Ts&&... ts) const
  {
    return f_(std::forward<Ts>(ts)...);
  }

  move_only_t(F&& f) : f_(std::move(f)) {}

  move_only_t(move_only_t&&) = default;
  move_only_t(const move_only_t&) = delete;
};

template <typename F>
auto move_only(F&& f)
{
  return move_only_t<F>(std::move(f));
}

TEST_CASE("vector of ints with lambdas")
{
  const std::vector<int> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  REQUIRE(rng::all_of(data, [](auto x) { return x < 10; }));
  
  REQUIRE(rng::any_of(data, [](auto x) { return x > 6; }));
  
  REQUIRE(rng::none_of(data, [](auto x) { return x > 10; }));
  
  REQUIRE(rng::count_if(data, [](auto x) { return x % 2 == 0; }) == 5);

  REQUIRE(rng::reduce(data, [](auto sum, auto x) { return sum + x; }) == 45);

  REQUIRE(rng::map(data, [](auto x) { return std::pow(2, x); })[9] == 512);
}

TEST_CASE("vector of ints with move_only wrapped lambdas")
{
  const std::vector<int> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  REQUIRE(rng::all_of(data, move_only([](auto x) { return x < 10; })));

  REQUIRE(rng::any_of(data, move_only([](auto x) { return x > 6; })));

  REQUIRE(rng::none_of(data, move_only([](auto x) { return x > 10; })));

  REQUIRE(rng::count_if(data, move_only([](auto x) { return x % 2 == 0; })) == 5);

  REQUIRE(rng::reduce( data, move_only([](auto sum, auto x) { return sum + x; })) == 45);

  REQUIRE(rng::map(data, move_only([](auto x) { return std::pow(2, x); }))[9] == 512);

}


