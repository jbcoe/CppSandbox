// std::tuple unpacking using a std::tuple indexer
//
// Adapted from: http://loungecpp.wikidot.com/tips-and-tricks%3aindices
//
// std::tuple<A,B,C> result =
// 	std::tuple<A,B,C>(f(get<0>(x), f(get<1>(x), f(get<2>(x));
//
//	Is replaced with
//
// std::tuple<Args...> result = std::tuple<Args...>(f(get<Indices>(x))...);
//

#include <iostream>
#include <tuple>

template <std::size_t... Indices>
struct TupleIndices
{
};

template <std::size_t N, std::size_t... Indices>
struct build_indices : build_indices<N - 1, N - 1, Indices...>
{
};

template <std::size_t... Indices>
struct build_indices<0, Indices...> : TupleIndices<Indices...>
{
};

template <typename... Ts>
using IndicesFor = build_indices<sizeof...(Ts)>;


template <typename Functor, typename Tuple_t, std::size_t... Indices>
Tuple_t apply_f_impl(Functor f, const Tuple_t& t, TupleIndices<Indices...>)
{
  return Tuple_t(f(std::get<Indices>(t))...);
}

template <typename Functor, typename... Ts>
std::tuple<Ts...> apply_f(Functor f, const std::tuple<Ts...>& t)
{
  return apply_f_impl(f, t, IndicesFor<Ts...>{});
}

struct MyFunctor
{
  template <typename T>
  T operator()(const T& t) const
  {
    return 2 * t;
  }
};

struct ToStream
{
  ToStream(std::ostream& os) : m_os(os)
  {
  }
  template <typename T>
  T operator()(T t)
  {
    m_os << t << ",";
    return t;
  }

  std::ostream& m_os;
};

template <typename... Ts>
std::ostream& operator<<(std::ostream& os, std::tuple<Ts...> t)
{
  os << '(';
  apply_f(ToStream(os), t);
  os << "\b)";
  return os;
}

int main()
{
  std::tuple<int, double, char> x(5, 1.5, 'a');
  auto y = apply_f(MyFunctor(), x);

  std::cout << "Before: " << x << " After: " << y << std::endl;
}
