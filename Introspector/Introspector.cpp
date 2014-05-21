#include <iostream>
#include <algorithm>
#include <functional>

template <typename T, typename Member_t>
struct Introspector
{
  static const std::function<Member_t&(T*)> fPtrs[];

  template <typename Functor_t>
  static void RunFunction(T* t, Functor_t& theFunction)
  {
    std::for_each(std::begin(fPtrs), std::end(fPtrs),
                  [&](std::function<Member_t&(T*)> f)
                  { theFunction(f(t)); });
  }
};

struct CSomeData : Introspector<CSomeData, int>
{
  int m_age = 32;
  int m_number = 4;
  int m_prime = 5;

  template <typename Functor_t>
  void RunFunction(Functor_t& theFunction)
  {
    return Introspector<CSomeData, int>::RunFunction(this, theFunction);
  }
};

template <>
const std::function<int&(CSomeData*)> Introspector<CSomeData, int>::fPtrs[] = {
    &CSomeData::m_age, &CSomeData::m_number, &CSomeData::m_prime};

int main(int argc, char* argv[])
{
  CSomeData myData;
  auto lambda_printer = [](int theInt)
  { std::cout << theInt << std::endl; };
  myData.RunFunction(lambda_printer);

  int total = 0;
  auto lambda_summer = [&total](int theInt)
  { total += theInt; };
  myData.RunFunction(lambda_summer);

  std::cout << "The sum is " << total << std::endl;
}
