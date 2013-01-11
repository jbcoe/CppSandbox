#include <iostream>
#include <sstream>
#include <vector>
#include <tbb/task_group.h>
#include <tbb/task_scheduler_init.h>
#include <Common/Timer.h>

template <typename TaskGenerator_t>
void RunTasks(size_t tasks, TaskGenerator_t gen)
{
  for (size_t i=1; i<=16; ++i)
	{
		std::stringstream ss;
		ss << i << '\t';

    Timer t(ss.str().c_str());

		tbb::task_scheduler_init tsInit(i);
		tbb::task_group tg;
		for ( size_t task=0; task<tasks; ++task )
		{
    	tg.run(gen());
		}
		tg.wait();
	}
}

int main(int argc, char* argv[])
{

	auto taskGenerator = []()
	{
		return []()
		{
			double r = 3.1;
			double x = 0.6;
			for (int i=0; i<100000; ++i)
			{
				x = r * x * ( 1.0 - x ); 
			}
			if ( x > 5 ) std::cout << "FAIL" << std::endl;
		};
	};

	RunTasks(1000, taskGenerator);
}

