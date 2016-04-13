#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>
#include <vector>

namespace rng
{
  template <typename R, typename F>
  bool any_of(const R&& r, F&& f)
  {
    using std::begin;
    using std::end;
    return std::any_of(begin(r), end(r), std::forward<F>(f));
  }
  
  template <typename R, typename F>
  bool all_of(const R&& r, F&& f)
  {
    using std::begin;
    using std::end;
    return std::all_of(begin(r), end(r), std::forward<F>(f));
  }
  
  template <typename R, typename F>
  bool none_of(const R&& r, F&& f)
  {
    using std::begin;
    using std::end;
    return std::none_of(begin(r), end(r), std::forward<F>(f));
  }
  
  template <typename R, typename F>
  size_t count_if(const R&& r, F&& f)
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

int main(int argc, char* argv[])
{
  std::vector<int> xs = {1,2,3,4,5,6,7,8,9,10};
  auto pow = rng::map(xs, [](auto x) { return std::pow(2, x); });
  auto q = rng::reduce(pow, std::plus<>());
  std::cout << q << '\n';
  return 0;
}

