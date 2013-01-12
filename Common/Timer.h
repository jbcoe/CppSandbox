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
 		f((end-start).total_milliseconds());
 	}

private:

 	F_t f;
 	ptime start;
};


struct TimingReport
{
  TimingReport(const std::string& e) : event(e) {}

  void operator()(long t) const { std::cout << event << " (ms) " << t <<std::endl; }

private:
	std::string event;
};

template <typename F_t>
TTimer<F_t> make_timer(F_t f)
{
	return TTimer<F_t>(f);
}

TTimer<TimingReport> make_timer(const char* s) { return make_timer(TimingReport(s)); }
TTimer<TimingReport> make_timer(const std::string& s) { return make_timer(TimingReport(s)); }

class Timer : TTimer<TimingReport>
{
	public:
	Timer(const char* s) : TTimer(TimingReport(s)) {}
	Timer(const std::string s) : TTimer(TimingReport(s)) {}
};
