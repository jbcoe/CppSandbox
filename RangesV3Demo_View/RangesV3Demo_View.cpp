#include <iostream>
#include <vector>
#include <range/v3/core.hpp>
#include <range/v3/view.hpp>

using namespace ranges;

int main(int argc, char* argv[])
{                          
  std::vector<int> xs = {0,1,2,3,4,5,6,7,8,9};
  for ( auto x : xs | view::remove_if([](int x){return x%2==0;}) | view::transform([](int x){return x*x;})) std::cout << x << ' ';
  std::cout << std::endl;
}

