// http://blogs.msdn.com/b/oldnewthing/archive/2015/01/16/10586692.aspx
#include <array>
#include <algorithm>
#include <iostream>
#include <random>
#include <Common/Timer.h>

class Buffer
{
public:
  void StoreInt(int64_t t)
  {
    *reinterpret_cast<int64_t*>(data_) = t;
  }

private:
  char data_[sizeof(int64_t)];
};

template <size_t N>
class MisalignedBuffer
{
public:
  void StoreInt(int64_t t)
  {
    *reinterpret_cast<int64_t*>(data_) = t;
  }

private:
  char padding_[N];
  char data_[sizeof(int64_t) * SIZE];
};

#define ALIGNMENT(x) std::cout << "alignof(" #x ") = " << alignof(x) << '\n';

int main(int argc, char* argv[])
{
  /*
  ALIGNMENT(char);
  ALIGNMENT(int);
  ALIGNMENT(size_t);
  ALIGNMENT(int64_t);
  ALIGNMENT(float);
  ALIGNMENT(double);
  ALIGNMENT(std::max_align_t);
  */
  const size_t LOOPS = 1e7;

  std::mt19937 engine;
  std::uniform_int_distribution<int64_t> distribution(0, 1000);
  auto gen = [&]
  {
    return distribution(engine);
  };

  std::vector<int> numbers(LOOPS);

  std::generate(numbers.begin(), numbers.end(), gen);

  {
    auto t = Timer("Buffer");
    Buffer b;
    for (size_t i = 0; i < LOOPS; ++i)
    {
      b.StoreInt(numbers[i]);
    }
  }

  {
    auto t = Timer("MisalignedBuffer<1>");
    MisalignedBuffer<1> b;
    for (size_t i = 0; i < LOOPS; ++i)
    {
      b.StoreInt(numbers[i]);
    }
  }

  {
    auto t = Timer("MisalignedBuffer<2>");
    MisalignedBuffer<2> b;
    for (size_t i = 0; i < LOOPS; ++i)
    {
      b.StoreInt(numbers[i]);
    }
  }

  {
    auto t = Timer("MisalignedBuffer<3>");
    MisalignedBuffer<3> b;
    for (size_t i = 0; i < LOOPS; ++i)
    {
      b.StoreInt(numbers[i]);
    }
  }

  {
    auto t = Timer("MisalignedBuffer<4>");
    MisalignedBuffer<4> b;
    for (size_t i = 0; i < LOOPS; ++i)
    {
      b.StoreInt(numbers[i]);
    }
  }

  {
    auto t = Timer("MisalignedBuffer<5>");
    MisalignedBuffer<5> b;
    for (size_t i = 0; i < LOOPS; ++i)
    {
      b.StoreInt(numbers[i]);
    }
  }

  {
    auto t = Timer("MisalignedBuffer<", 6, ">");
    MisalignedBuffer<6> b;
    for (size_t i = 0; i < LOOPS; ++i)
    {
      b.StoreInt(numbers[i]);
    }
  }
}
