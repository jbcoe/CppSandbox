#include <cassert>
#include <iostream>

#define ASSERT_X(condition,lambda)\
  assert( condition || []{ lambda; return false; }() );

int f(int x)
{
  ASSERT_X( x==5, std::cerr << "x!=5\n" );
  return x;
}

int main()
{
  f(4);
}
