#include <iostream>
#include <typeinfo>
#include <vector>
#include <tuple>

template<typename T, typename... Ts>
struct MultiVectorBase
{
  using Data = decltype(
      std::tuple_cat(
        std::tuple<std::vector<T>>(),
        std::declval<typename MultiVectorBase<Ts...>::Data>() 
        )
      );
};

template<typename T>
struct MultiVectorBase<T>
{
  using Data = std::tuple<std::vector<T>>;
};

template<size_t N, size_t Max>
struct Pack
{
  template<typename... Ts, typename... Vs>
  static void DoPack(std::tuple<Ts...>& tpl, const std::tuple<Vs...>& vals)
  {
    std::get<N>(tpl).push_back(std::get<N>(vals));
    Pack<N+1, Max>::DoPack(tpl, vals);
  }
};

template<size_t Max>
struct Pack<Max, Max>
{
  template<typename... Ts, typename... Vs>
  static void DoPack(std::tuple<Ts...>& tpl, const std::tuple<Vs...>& vals)
  {
  }
};

template<typename... Ts>
struct MultiVector
{
  typename MultiVectorBase<Ts...>::Data data_;
  
  template<typename... Vs>
  void push_back(const Vs&... vs)
  { 
    static_assert(sizeof...(Ts)==sizeof...(Vs), "Argument count mismatch");
    Pack<0,sizeof...(Ts)>::DoPack(data_, std::make_tuple(vs...));
  }
};

template<size_t N, typename... Ts>
const auto& get(MultiVector<Ts...>& mv)
{
  return std::get<N>(mv.data_);
}

int main(int argc, char* argv[])
{
  MultiVector<int,double,char> mv;

  mv.push_back(1,2.718281,'e');
  mv.push_back(2,3.14159,'p');
  mv.push_back(3,2.718281,'e');
  mv.push_back(4,3.14159,'p');

  const std::vector<int>& ints = get<0>(mv);
  const std::vector<double>& dbls = get<1>(mv);
  const std::vector<char>& chars = get<2>(mv);

  std::copy(ints.begin(),ints.end(),std::ostream_iterator<int>(std::cout," "));
  std::cout << '\n';
  std::copy(dbls.begin(),dbls.end(),std::ostream_iterator<double>(std::cout," "));
  std::cout << '\n';
  std::copy(chars.begin(),chars.end(),std::ostream_iterator<char>(std::cout," "));
  std::cout << '\n';
}

