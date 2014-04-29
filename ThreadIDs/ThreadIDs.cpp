#include <iostream>
#include <thread>
#include <mutex>

class ScopedThread
{
  std::thread t;

public:
  template <typename F_t>
  ScopedThread(F_t f)
      : t(std::thread(f))
  {
  }
  ~ScopedThread()
  {
    if (t.joinable()) t.join();
  }

  ScopedThread(const ScopedThread&) = delete;
  ScopedThread operator=(const ScopedThread&) = delete;
};

std::mutex cout_mutex;

int main(int argc, char* argv[])
{
  auto master_thread_id = std::this_thread::get_id();

  auto lambda_f = [=]()
  {
    auto thread_id = std::this_thread::get_id();
    if (thread_id == master_thread_id)
    {
      std::lock_guard<std::mutex> l(cout_mutex);
      std::cout << "Master thread. ID=" << thread_id << std::endl;
    }
    else
    {
      std::lock_guard<std::mutex> l(cout_mutex);
      std::cout << "Child thread.  ID=" << thread_id << std::endl;
    }
  };

  ScopedThread t1(lambda_f);
  ScopedThread t2(lambda_f);
  ScopedThread t3(lambda_f);
  ScopedThread t4(lambda_f);
  ScopedThread t5(lambda_f);

  lambda_f();
}
