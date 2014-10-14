#include <atomic>
#include <iterator>
#include <algorithm>
#include <vector>
#include <iostream>
#include <future>

int main(int argc, char* argv[])
{
  std::atomic<size_t> completionCount;
  completionCount = 0;

  std::vector<std::future<size_t>> futures(100);
  for (size_t i = 0; i < 100; ++i)
  {
    futures[i] = std::async([&completionCount]()
                            {
                              return ++completionCount;
                            });
  }

  for (size_t i = 0; i < futures.size(); ++i)
  {
    std::cout << futures[i].get();
    if (i < futures.size() - 1)
    {
      std::cout << ", ";
    }
  }
  std::cout << std::endl;
}
