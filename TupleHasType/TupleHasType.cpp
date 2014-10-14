#include <iostream>
#include <tuple>
#include <type_traits>

template <bool b, typename T, typename U>
struct if_type_else_type
{
};

template <typename T, typename U>
struct if_type_else_type<true, T, U>
{
  typedef T type;
};

template <typename T, typename U>
struct if_type_else_type<false, T, U>
{
  typedef U type;
};

template <typename T, typename TupleT, size_t n = 0,
          size_t tuple_size = std::tuple_size<TupleT>::value>
struct TupleContainsType
{
  typedef typename std::tuple_element<n, TupleT>::type TypeN;
  typedef typename if_type_else_type<
      std::is_same<T, TypeN>::value, std::true_type,
      typename TupleContainsType<T, TupleT, n + 1>::type>::type type;

  enum
  {
    value = type::value
  };
};

template <typename T, typename TupleT, size_t N>
struct TupleContainsType<T, TupleT, N, N>
{
  typedef typename std::false_type::type type;

  enum
  {
    value = false
  };
};

int main(int argc, char* argv[])
{
  typedef std::tuple<int, char, float> T;
  std::cout << "std::tuple<int,char,float> contains int "
            << TupleContainsType<int, T>::value << "\n";
  std::cout << "std::tuple<int,char,float> contains char "
            << TupleContainsType<char, T>::value << "\n";
  std::cout << "std::tuple<int,char,float> contains float "
            << TupleContainsType<float, T>::value << "\n";
  std::cout << "std::tuple<int,char,float> contains double "
            << TupleContainsType<double, T>::value << "\n";
  std::cout << "std::tuple<int,char,float> contains bool "
            << TupleContainsType<bool, T>::value << "\n";

  return 0;
}
