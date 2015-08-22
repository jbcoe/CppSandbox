#pragma once

#include <iostream>
#include <chrono>
#include <sstream>


template <typename F_t>
class TTimer
{
  using clock = std::chrono::high_resolution_clock;

public:
  TTimer(F_t f) : f_(f)
  {
    start_ = clock::now();
  }

  ~TTimer()
  {
    if ( !engaged_) { return; }
    auto end = clock::now();
    f_(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_)
          .count());
  }
  
  TTimer(TTimer&& t) : f_(std::move(t.f_)), start_(t.start_)
  {
    t.engaged_ = false;
  }

  TTimer(const TTimer&) = delete;

private:
  F_t f_;
  clock::time_point start_;
  bool engaged_ = true;
};

template <typename T>
void AppendToStream(std::ostream& s, T t)
{
  s << t;
}

template <typename T, typename... Ts>
void AppendToStream(std::ostream& s, T t, Ts... ts)
{
  s << t;
  AppendToStream(s, ts...);
}

struct TimingReport
{
  template <typename... Ts>
  TimingReport(Ts... ts)
  {
    std::stringstream ss;
    AppendToStream(ss, ts...);
    event = ss.str();
  }

  template <typename T>
  void operator()(T t) const
  {
    std::cout << event << " " << t << " us" << std::endl;
  }

private:
  std::string event;
};

template <typename F_t>
TTimer<F_t> make_timer(F_t f)
{
  return TTimer<F_t>(f);
}

TTimer<TimingReport> make_timer(const char* s)
{
  return make_timer(TimingReport(s));
}
TTimer<TimingReport> make_timer(const std::string& s)
{
  return make_timer(TimingReport(s));
}

template <typename... Ts>
TTimer<TimingReport> make_timer(Ts... ts)
{
  return make_timer(TimingReport(ts...));
}

class Timer : TTimer<TimingReport>
{
public:
  Timer(const char* s) : TTimer(TimingReport(s))
  {
  }
  Timer(const std::string s) : TTimer(TimingReport(s))
  {
  }
};

