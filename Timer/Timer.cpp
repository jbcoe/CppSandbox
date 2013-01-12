#include <iostream>
#include <algorithm>
#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace posix_time;

template <typename F_t>
class Timer
{
public:

	Timer(F_t f_) : f(f_) 
	{
		start = microsec_clock::local_time();
	}

 	~Timer()
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
Timer<F_t> make_timer(F_t f)
{
	return Timer<F_t>(f);
}

Timer<TimingReport> make_timer(const char* s) { return make_timer(TimingReport(s)); }
Timer<TimingReport> make_timer(const std::string& s) { return make_timer(TimingReport(s)); }

int main(int argc, char* argv[])
{
	auto t(make_timer([](long t){std::cout << "Time passed: " << t << std::endl; }));
	auto t2(make_timer("Build vector"));
	std::vector<int> v(10000000);
	assert(std::find(v.begin(), v.end(), 9) == v.end());
}



