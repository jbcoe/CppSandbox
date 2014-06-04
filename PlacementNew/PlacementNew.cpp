#include <iostream>
#include <array>
#include <memory>

////////////////////////////////////////////////////////////

class MyClass
{
public:
  int m_iInt;
  double m_dbl;
  MyClass() {}
};

////////////////////////////////////////////////////////////

class MyOtherClass
{
public:
  double m_dbl;
  int m_iInt;
  MyOtherClass() {}
};

////////////////////////////////////////////////////////////

template <typename T, typename U, typename... Vs>
struct MaxSizeOf
{
  enum
  { value = sizeof(T) >= sizeof(U) ? MaxSizeOf<T, Vs...>::value
                                   : MaxSizeOf<U, Vs...>::value };
};

template <typename T, typename U>
struct MaxSizeOf<T, U>
{
  enum
  { value = sizeof(T) >= sizeof(U) ? sizeof(T) : sizeof(U) };
};

template <typename... ConstructedTypes>
class MyBlock
{
  enum
  { N = MaxSizeOf<char, ConstructedTypes...>::value };

  std::array<char, N> m_data;

  struct Deleter
  {
    template <typename T>
    void operator()(T* pT)
    {
      pT->~T();
    }
  };

public:
  template <typename T, typename... Ts>
  std::unique_ptr<T, Deleter> InstanceOf(Ts... ts)
  {
    static_assert(sizeof(T) <= N, "Not enough space in block");
    return std::unique_ptr<T, Deleter>(new (m_data.data())
                                       T(std::forward(ts)...));
  }
};

////////////////////////////////////////////////////////////

int main()
{
  MyBlock<MyClass, MyOtherClass> block;

  auto pMyInstance = block.InstanceOf<MyClass>();

  pMyInstance->m_iInt = 8;
  pMyInstance->m_dbl = 9.3;

  auto pMySecondInstance = block.InstanceOf<MyOtherClass>();

  std::cout << pMyInstance->m_dbl << std::endl;
  std::cout << pMyInstance->m_iInt << std::endl;

  std::cout << pMySecondInstance->m_dbl << std::endl;
  std::cout << pMySecondInstance->m_iInt << std::endl;
}

////////////////////////////////////////////////////////////
