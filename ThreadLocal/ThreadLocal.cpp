#include <iostream>
#include <future>
#include <vector>

thread_local int instanceCount = 0;

class TaskCommon
{
public:
  TaskCommon()
  {
    ++instanceCount;
  }
};

int main(int argc, char* argv[])
{
  int counts[] = {0, 0};

  std::vector<std::thread> threads;

  threads.emplace_back([&]()
                       {
                         TaskCommon a;
                         TaskCommon b;
                         TaskCommon c;
                         counts[0] = instanceCount;
                       });

  int count2 = 0;

  threads.emplace_back([&]()
                       {
                         TaskCommon a;
                         TaskCommon b;
                         counts[1] = instanceCount;
                       });

  for (auto& t : threads)
  {
    t.join();
  }

  std::cout << "Thread 1 created " << counts[0] << " objects" << std::endl;
  std::cout << "Thread 2 created " << counts[1] << " objects" << std::endl;

  /*
  for ( auto& result : threadTaskCounts )
  {
          static int i=0;
          std::cout << "Thread " << i++ << " used " << result.get() << " task
  objects" << std::endl;
  }
  */
}
