#include <iostream>
#include <functional>

int add(int x, int y) { return x+y; }

using namespace std::placeholders;

int main(int argc, char* argv[])
{
  auto g = std::bind(add, _2, _1);
  std::cout << g(40,2) << '\n';
}

