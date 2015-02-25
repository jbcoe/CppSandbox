#include <iostream>

int f()
{
  return 1;
}

int noexcept_f() noexcept
{
  return 1;
}

auto g()
{
  return noexcept_f();
}

auto conditional_noexcept_g() noexcept(noexcept(noexcept_f()))
{
  return noexcept_f();
}

int main(int argc, char* argv[])
{
  std::cout << std::boolalpha << "f is noexcept: " << noexcept(f())
            << std::endl;
  std::cout << std::boolalpha
            << "noexcept_f is noexcept: " << noexcept(noexcept_f())
            << std::endl;
  std::cout << std::boolalpha << "g is noexcept: " << noexcept(g())
            << std::endl;
  std::cout << std::boolalpha << "conditional_noexcept_g is noexcept: "
            << noexcept(conditional_noexcept_g()) << std::endl;
}
