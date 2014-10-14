// tuple utils taken from Jonathan Wakeley
// https://gitorious.org/redistd/redistd/source/0d9c849726729e66e5703fadba0222d6df724533:include/redi/index_tuple.h#L12
// http://stackoverflow.com/questions/10604794/convert-stdtuple-to-stdarray-c11

#include <iostream>
#include <tuple>
#include <array>

//////////////////////////////////////////////////////////

template <unsigned... Indices>
struct index_tuple
{
  template <unsigned N>
  using append = index_tuple<Indices..., N>;
};

template <unsigned Size>
struct make_index_tuple
{
  typedef
      typename make_index_tuple<Size - 1>::type::template append<Size - 1> type;
};

template <>
struct make_index_tuple<0>
{
  typedef index_tuple<> type;
};

template <typename... Types>
using to_index_tuple = typename make_index_tuple<sizeof...(Types)>::type;

//////////////////////////////////////////////////////////

template <typename T, typename... U>
using Array = std::array<T, 1 + sizeof...(U)>;

template <typename T, typename... U, unsigned... I>
inline Array<T, U...> tuple_to_array2(const std::tuple<T, U...>& t,
                                      index_tuple<I...>)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-braces"
  return Array<T, U...>{std::get<I>(t)...};
#pragma clang diagnostic pop
}

template <typename T, typename... U>
inline Array<T, U...> tuple_to_array(const std::tuple<T, U...>& t)
{
  using IndexTuple = typename make_index_tuple<1 + sizeof...(U)>::type;
  return tuple_to_array2(t, IndexTuple());
}

//////////////////////////////////////////////////////////

template <size_t N, size_t I = 0>
struct PackTuple
{
  template <typename T, typename... Ts>
  static void Pack(const std::array<T, N>& array, std::tuple<Ts...>& t)
  {
    std::get<I>(t) = array[I];
    PackTuple<N, I + 1>::Pack(array, t);
  }
};

template <size_t N>
struct PackTuple<N, N>
{
  template <typename T, typename... Ts>
  static void Pack(const std::array<T, N>& array, std::tuple<Ts...>& t)
  {
  }
};

//////////////////////////////////////////////////////////

template <typename... Ts>
void SortTuple(std::tuple<Ts...>& ts)
{
  auto array = tuple_to_array(ts);
  std::sort(array.begin(), array.end());
  PackTuple<sizeof...(Ts)>::Pack(array, ts);
}

//////////////////////////////////////////////////////////

template <size_t N, size_t I = 0>
struct PrintTuple
{
  template <typename... Ts>
  static void Print(const std::tuple<Ts...>& t)
  {
    std::cout << std::get<I>(t) << " ";
    PrintTuple<N, I + 1>::Print(t);
  }
};

template <size_t N>
struct PrintTuple<N, N>
{
  template <typename... Ts>
  static void Print(const std::tuple<Ts...>& t)
  {
    std::cout << std::endl;
  }
};

template <typename... Ts>
void Print(const std::tuple<Ts...>& t)
{
  PrintTuple<sizeof...(Ts)>::Print(t);
}

//////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
  auto myInts = std::make_tuple(10, 11, 9, 8, 7, 3, 2, 1, 4, 6, 5, 0);
  Print(myInts);
  SortTuple(myInts);
  Print(myInts);
}
