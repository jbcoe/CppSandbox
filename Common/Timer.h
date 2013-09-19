#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace posix_time;

template <typename F_t>
class TTimer
{
public:

  TTimer(F_t f_) : f(f_)
  {
    start = microsec_clock::local_time();
  }

  ~TTimer()
  {
    auto end = microsec_clock::local_time();
    f((end - start).total_milliseconds());
  }

private:

  F_t f;
  ptime start;
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

  void operator()(long t) const
  {
    std::cout << event << " (ms) " << t << std::endl;
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
