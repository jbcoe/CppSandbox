#include <iostream>
#include <array>
#include <iterator>
#include <algorithm>
#include <random>
#include <Common/Timer.h>

template <int BIG, typename Ints_t>
int SumOfBigNumbers(const Ints_t& ints, const size_t iterations,
                    const char* message)
{
  auto t = make_timer(message);
  int bigSum = 0;
  for (size_t iterate = 0; iterate < iterations; ++iterate)
  {
    std::for_each(ints.begin(), ints.end(), [&](int i)
                  {
                    if (i > BIG)
                    {
                      bigSum += i;
                    }
                  });
  }
  return bigSum;
}

int main()
{
  enum
  {
    COUNT = 10000
  };
  enum
  {
    MAX = 100
  };
  enum
  {
    BIG = 50
  };

  size_t repeats = 100000;

  std::uniform_int_distribution<int> distribution(0, MAX);
  std::mt19937 engine;
  auto generator = std::bind(distribution, engine);

  std::array<int, COUNT> myInts;
  std::generate(myInts.begin(), myInts.end(), generator);

  int a = SumOfBigNumbers<BIG>(myInts, repeats, "Unsorted");

  std::sort(myInts.begin(), myInts.end());

  int b = SumOfBigNumbers<BIG>(myInts, repeats, "Sorted");

  return !(a == b);
}
