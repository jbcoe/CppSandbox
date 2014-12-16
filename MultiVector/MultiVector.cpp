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
struct PushBack
{
  template<typename... Ts, typename... Vs>
  static void DoPushBack(std::tuple<Ts...>& tpl, const std::tuple<Vs...>& vals)
  {
    std::get<N>(tpl).push_back(std::get<N>(vals));
    PushBack<N+1, Max>::DoPushBack(tpl, vals);
  }
};

template<size_t Max>
struct PushBack<Max, Max>
{
  template<typename... Ts, typename... Vs>
  static void DoPushBack(std::tuple<Ts...>& tpl, const std::tuple<Vs...>& vals)
  {
  }
};

template<size_t N, size_t Max>
struct BuildReferenceTuple
{
  template<typename... Ts>
  static auto DoBuildReferenceTuple(std::tuple<Ts...>& tpl, size_t i)
  {
    return std::tuple_cat(std::forward_as_tuple(std::get<N>(tpl)[i]), 
          BuildReferenceTuple<N+1,Max>::DoBuildReferenceTuple(tpl,i));
  }
};

template<size_t Max>
struct BuildReferenceTuple<Max, Max>
{
  template<typename... Ts>
  static auto DoBuildReferenceTuple(std::tuple<Ts...>& tpl, size_t i)
  {
    return std::make_tuple();
  }
};

template<size_t N, size_t Max>
struct BuildConstReferenceTuple
{
  template<typename... Ts>
  static auto DoBuildConstReferenceTuple(const std::tuple<Ts...>& tpl, size_t i)
  {
    return std::tuple_cat(std::forward_as_tuple(std::get<N>(tpl)[i]), 
          BuildConstReferenceTuple<N+1,Max>::DoBuildReferenceTuple(tpl,i));
  }
};

template<size_t Max>
struct BuildConstReferenceTuple<Max, Max>
{
  template<typename... Ts>
  static auto DoBuildConstReferenceTuple(const std::tuple<Ts...>& tpl, size_t i)
  {
    return std::make_tuple();
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
    PushBack<0,sizeof...(Ts)>::DoPushBack(data_, std::make_tuple(vs...));
  }

  std::tuple<Ts&...> operator[](size_t i)
  {
    return BuildReferenceTuple<0,sizeof...(Ts)>::DoBuildReferenceTuple(data_, i);
  }

  std::tuple<const Ts&...> operator[](size_t i) const
  {
    return BuildConstReferenceTuple<0,sizeof...(Ts)>::DoBuildConstReferenceTuple(data_, i);
  }
  
  size_t size() const { return data_.size(); }
};

template<size_t N, typename... Ts>
auto& get(MultiVector<Ts...>& mv)
{
  return std::get<N>(mv.data_);
}

template<size_t N, typename... Ts>
const auto& get(const MultiVector<Ts...>& mv)
{
  return std::get<N>(mv.data_);
}

int main(int argc, char* argv[])
{
  MultiVector<int,double,char> mv;

  mv.push_back(1,2.718281,'e');
  mv.push_back(2,3.14159,'p');
  mv.push_back(3,4,'x');
  mv.push_back(4,5,'y');

  const std::vector<int>& ints = get<0>(mv);
  const std::vector<double>& dbls = get<1>(mv);
  const std::vector<char>& chars = get<2>(mv);

  std::copy(ints.begin(),ints.end(),std::ostream_iterator<int>(std::cout," "));
  std::cout << '\n';
  std::copy(dbls.begin(),dbls.end(),std::ostream_iterator<double>(std::cout," "));
  std::cout << '\n';
  std::copy(chars.begin(),chars.end(),std::ostream_iterator<char>(std::cout," "));
  std::cout << '\n';

  auto x = mv[0];
  std::get<0>(x)=99;
  std::copy(ints.begin(),ints.end(),std::ostream_iterator<int>(std::cout," "));
  std::cout << '\n';
}

