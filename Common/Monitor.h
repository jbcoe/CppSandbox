#ifndef __USERS_JON_DEV_CPPSANDBOX_COMMON_MONITOR_H
#define __USERS_JON_DEV_CPPSANDBOX_COMMON_MONITOR_H

#include <thread>

template <typename T>
class Monitor
{
  T t;
  std::mutex m;

public:
  Monitor(T _t) : t(_t)
  {
  }
  Monitor()
  {
  }

  template <typename F>
  auto operator()(F f) -> decltype(f(t))
  {
    std::lock_guard<std::mutex> l(m);
    return f(t);
  }
};

#endif
