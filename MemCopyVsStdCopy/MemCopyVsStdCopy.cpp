#include <iostream>
#include <algorithm>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <Common/Timer.h>
#include <random>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Requires an integer argument" << std::endl;
    return -1;
  }

  size_t size = boost::lexical_cast<size_t>(argv[1]);


  std::uniform_real_distribution<double> distribution(-1.0, 1.0);
  std::mt19937 engine;
  auto generator = [&]
  { return distribution(engine); };

  std::vector<double> values_a(size);
  values_a.resize(0);
  std::generate_n(std::back_inserter(values_a), size, generator);

  std::vector<double> values_b(size);
  std::vector<double> values_c(size);
  std::vector<double> values_d(size);

  {
    Timer t("iterative copy");
    for (size_t i = 0; i < size; ++i)
    {
      values_b[i] = values_a[i];
    }
  }

  {
    Timer t("memcopy");
    memcpy(&values_c[0], &values_a[0], size * sizeof(double));
  }

  {
    Timer t("std::copy");
    std::copy(values_a.begin(), values_a.end(), values_d.begin());
  }

  if (values_a == values_b && values_c == values_d)
    return 0;
  return -1;
}
