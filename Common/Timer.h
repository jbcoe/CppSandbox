#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace posix_time;

class Timer
{
	public:

		Timer(const char* eventName) : m_event(eventName) 
	{
		m_start = microsec_clock::local_time();
	}

		~Timer()
		{
			m_end = microsec_clock::local_time();
			std::cout << m_event << ' ' << (m_end - m_start).total_milliseconds() << std::endl;
		}

	private:

		std::string m_event;
		ptime m_start;
		ptime m_end;
};
