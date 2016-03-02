#include <iostream>
#include <functional>

int add(int x, int y)
{
  return x + y;
}

void accumulate(int value, int& sum)
{
  sum += value;
}

using namespace std::placeholders;

int main(int argc, char* argv[])
{
  auto f = [](auto && arg1, auto && arg2) { return add(arg1, arg2); };
  std::cout << f(40,2) << '\n';
  
  auto g = [](auto && arg1, auto && arg2) { return accumulate(arg1, arg2); };
  int x = 0;
  g(40,x);
  std::cout << x << '\n';
}

