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
  auto f = std::bind(add,_1,_2);
  std::cout << f(40,2) << '\n';
  
  auto g = std::bind(accumulate,_1,_2);
  int x = 0;
  g(40,x);
  std::cout << x << '\n';
}

