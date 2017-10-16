#include <iostream>
#include <range/v3/all.hpp>

using namespace ranges;


int main(int argc, char* argv[])
{
  for( auto x : view::ints(1) 
              | view::remove_if([](auto x) { return x % 2 == 0; }) 
              | view::transform([](auto x) { return x * x; })
              | view::take(10))
  {
    std::cout << x << ' ';
  }

  std::cout << std::endl;
}
