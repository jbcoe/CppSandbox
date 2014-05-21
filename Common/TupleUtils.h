#include <tuple>

class TupleUtils
{
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
  static auto apply_f_impl(Functor f, Tuple_t& t, TupleIndices<Indices...>)
      -> decltype(std::make_tuple(f(std::get<Indices>(t))...))
  {
    return std::make_tuple(f(std::get<Indices>(t))...);
  }

  struct ToStream
  {
    ToStream(std::ostream& os) : m_os(os) {}
    template <typename T>
    T operator()(T t)
    {
      m_os << t << ",";
      return t;
    }

    std::ostream& m_os;
  };

  template <typename... Ts>
  friend std::ostream& operator<<(std::ostream& os, std::tuple<Ts...> t);


public:
  template <typename Functor, typename... Ts>
  static auto ApplyFunction(Functor f, const std::tuple<Ts...>& t)
      -> decltype(apply_f_impl(f, t, IndicesFor<Ts...>()))
  {
    return apply_f_impl(f, t, IndicesFor<Ts...>());
  }

  template <typename Functor, typename... Ts>
  static std::tuple<Ts...> ApplyFunction(Functor f, std::tuple<Ts...>& t)
  {
    return apply_f_impl(f, t, IndicesFor<Ts...>());
  }
};


template <typename... Ts>
std::ostream& operator<<(std::ostream& os, std::tuple<Ts...> t)
{
  os << '(';
  TupleUtils::ApplyFunction(TupleUtils::ToStream(os), t);
  os << "\b)";
  return os;
}
