#include <iostream>
#include <algorithm>
#include <memory>
#include <iterator>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace posix_time;

typedef int COMPARED_TYPE;
static const COMPARED_TYPE COMPARED_VALUE = 1;

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
			std::cout << m_event << ' ' << m_end - m_start << std::endl;
		}

	private:

		std::string m_event;
		ptime m_start;
		ptime m_end;
};

class MySimpleClass
{
	public:
		double m_x = 0.0;
		double m_y = 0.0;

    static size_t g_moveCount;
    static size_t g_copyCount;

	MySimpleClass() 
	{
	}	
	
	MySimpleClass(const MySimpleClass& m) : m_x(m.m_x), m_y(m.m_y)
	{
		if ( g_copyCount == 0 )
		{
			++g_copyCount;
			std::cout << "Copy" << std::endl;
		}
	}
#ifdef USE_MOVE
	MySimpleClass(MySimpleClass&& m) : m_x(m.m_x), m_y(m.m_y)
	{
		if ( g_moveCount == 0 )
		{
			++g_moveCount;
			std::cout << "Move" << std::endl;
		}
	}
#endif
	void operator = (const MySimpleClass& m )
	{
		if ( g_copyCount == 0 )
		{
			++g_copyCount;
			std::cout << "Copy" << std::endl;
		}
		m_x = m.m_x;
		m_y = m.m_y;
	}
#ifdef USE_MOVE
	void operator = (MySimpleClass&& m )
	{
		if ( g_moveCount == 0 )
		{
			++g_moveCount;
			std::cout << "Move" << std::endl;
		}
		m_x = m.m_x;
		m_y = m.m_y;
	}
#endif
};

size_t MySimpleClass::g_copyCount = 0;
size_t MySimpleClass::g_moveCount = 0;

int main(int argc, char* argv[])
{                               
	size_t count = 10000000;

	if( argc == 2 )
		count = boost::lexical_cast<size_t>(argv[1]);

	{ 
		std::vector<std::unique_ptr<MySimpleClass>> vptrObjects;
		vptrObjects.reserve(count);
		{
			Timer t("Build pointer vector");
			std::generate_n(std::back_inserter(vptrObjects), count, [](){return std::unique_ptr<MySimpleClass>(new MySimpleClass());}); 
		}
		{
			Timer t("Modify Pointer Vector");
			std::for_each(vptrObjects.begin(), vptrObjects.end(), [](std::unique_ptr<MySimpleClass>& ptr){ptr->m_x = 2.0; ptr->m_y = 0.78 * ptr->m_x;});
		}		
		{	
			Timer t("Delete Pointer Vector");
			vptrObjects.resize(0);
		}
	}
  
	MySimpleClass::g_copyCount = 0;
	MySimpleClass::g_moveCount = 0;

	{
		std::vector<MySimpleClass> objects;
		objects.reserve(count);
		{
			Timer t("Build Object Vector");
			std::generate_n(std::back_inserter(objects), count, [](){return MySimpleClass();});
		}
		{
			Timer t("Modify Object Vector");
			std::for_each(objects.begin(), objects.end(), [](MySimpleClass& m){m.m_x = 2.0; m.m_y = 0.78 * m.m_x;});
		}
		{
			Timer t("Delete Object Vector");
			objects.resize(0);
		}
	}
}
