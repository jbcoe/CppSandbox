#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include <iostream>
#include "catch.hpp"

unsigned int Factorial(unsigned int number)
{
  return number <= 1 ? number : Factorial(number - 1) * number;
}

class Volatility
{
  double underlying_;

public:
  explicit Volatility(double u) : underlying_(u)
  {
  }
  explicit operator double() const
  {
    return underlying_;
  }
};

TEST_CASE("Implicit conversion performed for comparisons")
{
  auto v = Volatility(0.3);

  REQUIRE(static_cast<double>(v) == Approx(0.3));
}

TEST_CASE("Factorials are computed", "[factorial]")
{
  REQUIRE(Factorial(1) == 1);
  REQUIRE(Factorial(2) == 2);
  REQUIRE(Factorial(3) == 6);
  REQUIRE(Factorial(10) == 3628800);
}

TEST_CASE("Forced Fail", "[2+2=5]")
{
  REQUIRE((2 + 2) == 5);
}
