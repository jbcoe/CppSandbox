#include <iostream>
#include <range/v3/core.hpp>
#include <range/v3/view.hpp>
#include <range/v3/algorithm.hpp>

using namespace ranges;


int main(int argc, char* argv[])
{
  for_each(view::ints(1) | view::take(10) 
                         | view::remove_if([](auto x) { return x % 2 == 0; }) 
                         | view::transform([](auto x) { return x * x; }),
           [](auto x) { std::cout << x << ' '; });

  std::cout << std::endl;
}
