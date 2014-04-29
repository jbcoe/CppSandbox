#include <iostream>
#include <sstream>
#include <vector>
#include <tbb/task_group.h>
#include <tbb/task_scheduler_init.h>
#include <Common/Timer.h>

template <typename TaskGenerator_t>
void RunTasks(size_t tasks, TaskGenerator_t gen)
{

  long firstRunTime = 0;

  for (size_t i = 1; i <= 16; ++i)
  {
    auto t = make_timer([&firstRunTime, i](long t)
    {
      if (i == 1) firstRunTime = t;
      std::cout << i << ' ' << (double)firstRunTime / t << std::endl;
    });

    tbb::task_scheduler_init tsInit(i);
    tbb::task_group tg;
    for (size_t task = 0; task < tasks; ++task)
    {
      tg.run(gen());
    }
    tg.wait();
  }
  std::cout << std::endl;
}

int main(int argc, char* argv[])
{

  auto taskGenerator = []()
  {
    return []()
    {
      double r = 3.1;
      double x = 0.6;
      for (int i = 0; i < 10000; ++i)
      {
        x = r * x * (1.0 - x);
      }
      if (x > 5) std::cout << "FAIL" << std::endl;
    };
  };

  std::cout << "Logistic map" << std::endl;
  RunTasks(10000, taskGenerator);

  auto streamTaskGenerator = []()
  {
    return []()
    {
      std::stringstream ss;
      ss << 5 << 7 << "Hello" << 4.6 << -0.1;
      if (ss.str() == "Nicaragua") std::cout << "FAIL" << std::endl;
    };
  };

  std::cout << "build string with stringstream" << std::endl;
  RunTasks(100000, streamTaskGenerator);

  auto toStringTaskGenerator = []()
  {
    return []()
    {
      auto s = std::to_string(66.6);
      if (s == "Nicaragua") std::cout << "FAIL" << std::endl;
    };
  };

  std::cout << "build string with to_string" << std::endl;
  RunTasks(100000, streamTaskGenerator);
  std::mutex m;

  auto mutexTaskGenerator = [&m]()
  {
    return [&m]()
    { std::lock_guard<std::mutex> l(m); };
  };

  std::cout << "mutex lock" << std::endl;
  RunTasks(100000, mutexTaskGenerator);
}
