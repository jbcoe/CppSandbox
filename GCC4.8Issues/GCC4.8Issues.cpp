// Submitted as GCC bug http://gcc.gnu.org/bugzilla/show_bug.cgi?id=58366

#include <thread>
#include <vector>

int GetInteger()
{
  return 5;
}

class ThreadLocalGenerator
{
  std::function<int(void)> f_;

public:
  ThreadLocalGenerator()
  {
    f_ = std::bind(GetInteger);
  }

  int operator()()
  {
    return f_();
  }
};

thread_local ThreadLocalGenerator generator{};

enum
{
  BIG_COUNT = 10000
};

int main(int argc, char* argv[])
{
  int impossible_error_count = 0;

  std::vector<std::thread> generators;
  for (int i = 0; i < BIG_COUNT; ++i)
  {
    generators.push_back(std::thread([&]
                                     {
                                       // Prevent dead code optimisation by
                                       // using the result of generator()
                                       if (generator() == 42)
                                       {
                                         ++impossible_error_count;
                                       }
                                     }));
  }

  for (auto& t : generators)
  {
    t.join();
  }

  return 0;
}
