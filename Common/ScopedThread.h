#ifndef __USERS_JON_DEV_CPPSANDBOX_COMMON_SCOPEDTHREAD_H
#define __USERS_JON_DEV_CPPSANDBOX_COMMON_SCOPEDTHREAD_H

#include <thread>
class ScopedThread
{
  std::thread t_;

public:
  template <typename F_t>
  ScopedThread(F_t f)
      : t_(std::thread(f))
  {
  }

  ScopedThread(ScopedThread&& s) : t_(std::move(s.t_)) {}
  ScopedThread& operator=(ScopedThread&& s)
  {
    t_ = std::move(s.t_);
    return *this;
  }
  ~ScopedThread()
  {
    if (t_.joinable()) t_.join();
  }

  ScopedThread(const ScopedThread&) = delete;
  ScopedThread operator=(const ScopedThread&) = delete;
};

#endif
