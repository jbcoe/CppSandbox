#include <Common/Timer.h>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <memory>

class MemBlock
{
public:
  template <typename Generator_t>
  MemBlock(size_t size, Generator_t& g)
      : m_size(size)
  {
    m_rawMemory = (char*)malloc(size);
    for (size_t i = 0; i < (size) / sizeof(size_t); ++i)
    {
      *(size_t*)(m_rawMemory + i) = g();
    }
  }

  ~MemBlock()
  {
    free(m_rawMemory);
  }
  size_t m_size;
  char* m_rawMemory;
};

template <typename Generator_t>
double* GetDoublePointer(MemBlock& m, size_t length, Generator_t& g)
{
  auto position = g();
  while ((position + length * (sizeof(double) / (sizeof(char*)))) >= m.m_size)
  {
    position = g();
  }

  return reinterpret_cast<double*>(m.m_rawMemory + position);
}

int main(int argc, char* argv[])
{
  size_t size = boost::lexical_cast<size_t>(argv[1]);

  std::uniform_int_distribution<size_t> distribution(0, size / 10);
  std::mt19937 engine;
  auto generator = [&]
  {
    return distribution(engine);
  };

  MemBlock m(size, generator);

  size_t length = size / 10;

  auto theFunction = [](double a, double b, double c, double d)
  {
    return (a + b) / (c + d);
  };

  double sum1 = 0.0;
  {
    Timer t("Simple loops");
    for (size_t pass = 0; pass < 100; ++pass)
    {
      double* ws = GetDoublePointer(m, length, generator);
      double* xs = GetDoublePointer(m, length, generator);
      double* ys = GetDoublePointer(m, length, generator);
      double* zs = GetDoublePointer(m, length, generator);

      for (int i = 0; i < length; ++i)
      {
        sum1 += theFunction(ws[i], xs[i], ys[i], zs[i]);
      }
    }
  }

  double sum2 = 0.0;
  {
    Timer t("Zip iterators");
    for (size_t pass = 0; pass < 100; ++pass)
    {
      double* ws = GetDoublePointer(m, length, generator);
      double* xs = GetDoublePointer(m, length, generator);
      double* ys = GetDoublePointer(m, length, generator);
      double* zs = GetDoublePointer(m, length, generator);

      std::for_each(boost::make_zip_iterator(boost::make_tuple(ws, xs, ys, zs)),
                    boost::make_zip_iterator(boost::make_tuple(
                        ws + length, xs + length, ys + length, zs + length)),
                    [&](const boost::tuple<double, double, double, double>& t)
                    {
                      sum2 += theFunction(t.get<0>(), t.get<1>(), t.get<2>(),
                                          t.get<3>());
                    });
    }
  }

  return sum1 == sum2;
}
