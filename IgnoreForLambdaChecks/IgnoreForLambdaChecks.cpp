#include <iostream>
#include <functional>
#include <type_traits>

template <typename T>
void RunFunction(T t, int i)
{
  static_assert(std::is_assignable<std::function<std::string(int)>, T>::value,
                "Function/functor has wrong arguments or return type ");
  std::cout << t(i) << std::endl;
}

int main(int argc, char* argv[])
{
  RunFunction([](int x)
              { return "std::string (int)"; },
              5);
  RunFunction([](int x)
              { return 4.0; },
              5);
  assignment to std::ignore prevents this compiling
}
