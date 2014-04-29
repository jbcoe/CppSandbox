#include <iostream>
#include <thread>

std::mutex g_ioMutex;

void GlobalFunction()
{
  std::lock_guard<std::mutex> l(g_ioMutex);
  std::cout << "Global function" << std::endl;
}

class ScopedThread
{
  std::thread t_;

public:
  template <typename F_t>
  ScopedThread(F_t f)
      : t_(std::thread(f))
  {
  }

  ScopedThread(ScopedThread&& s) : t_(std::move(s.t_))
  {
  }
  ScopedThread operator=(ScopedThread&& s)
  {
    t_ = std::move(s.t_);
  }
  ~ScopedThread()
  {
    if (t_.joinable()) t_.join();
  }

  ScopedThread(const ScopedThread&) = delete;
  ScopedThread operator=(const ScopedThread&) = delete;
};

int main(int argc, char* argv[])
{
  ScopedThread myThreadWithLambda([]()
  {
    std::lock_guard<std::mutex> l(g_ioMutex);
    std::cout << "Lambda function" << std::endl;
  });
  ScopedThread myThreadWithGlobal(GlobalFunction);

  ScopedThread threadWithLambdaMove = std::move(myThreadWithLambda);
}
