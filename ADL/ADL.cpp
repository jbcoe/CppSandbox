#include <iostream>

namespace ADL
{
  class A
  {
  };

  template <typename... Ts>
  void foo(const Ts&...)
  {
    std::cout << "ADL::foo()" << std::endl;
  }
}

namespace OTHER
{
  class B
  {
  };

  template <typename... Ts>
  void foo(const Ts&...)
  {
    std::cout << "OTHER::foo()" << std::endl;
  }
}

class C
{
  template <typename... Ts>
  static void foo(const Ts&...)
  {
    std::cout << "C::foo()" << std::endl;
  }
};

int main(int argc, char* argv[])
{
  ADL::A a;
  foo(a);

  OTHER::B b;
  foo(b);

  ADL::foo(b);

  // foo(a,b); // ambiguous
  C c;
  // foo(c); // name not found
}
