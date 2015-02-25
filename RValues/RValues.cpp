#include <iostream>

void f(const int& i)
{
  std::cout << "  const int&\n";
}

void f(int& i)
{
  std::cout << "  int&\n";
}

void f(int&& i)
{
  std::cout << "  int&&\n";
}

int GetFive()
{
  return 5;
}

int main(int argc, char* argv[])
{
  std::cout << "int x = 5;" << std::endl;
  int x = 5;
  std::cout << "f(x);" << std::endl;
  f(x);
  std::cout << "f(5);" << std::endl;
  f(5);
  std::cout << "f(GetFive());" << std::endl;
  f(GetFive());
  std::cout << "f(int{});" << std::endl;
  f(int{});
  std::cout << "int& rx = x;" << std::endl;
  int& rx = x;
  std::cout << "f(rx);" << std::endl;
  f(rx);
  std::cout << "const int& crx = x;" << std::endl;
  const int& crx = x;
  std::cout << "f(crx);" << std::endl;
  f(crx);
  std::cout << "int&& rvx = std::move(x);" << std::endl;
  int&& rvx = std::move(x);
  std::cout << "f(rvx);" << std::endl;
  f(rvx);
  std::cout << "f(std::move(x));" << std::endl;
  f(std::move(x));
}
