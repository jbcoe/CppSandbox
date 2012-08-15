// See http://herbsutter.com/2009/05/15/effective-concurrency-eliminate-false-sharing/

#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <tbb/task_group.h>
#include <tbb/task_scheduler_init.h>
#include <boost/lexical_cast.hpp>
#include <Common/Timer.h>

class SquareMatrix
{
	public:
	SquareMatrix(size_t dim) : m_data(dim*dim, 0), m_dim(dim) {}
	double* operator[](size_t i)
	{
		return &m_data[i*m_dim];
	}

	std::vector<double>::iterator begin() { return m_data.begin(); }
	std::vector<double>::iterator end() { return m_data.end(); }

	private:
	std::vector<double> m_data;
	size_t m_dim;
};

int main(int argc, char* argv[])
{
  size_t THREADS = 4;
	if ( argc > 1 )
		THREADS = boost::lexical_cast<size_t>(argv[1]);
	
	size_t dim = 1024;
	if ( argc > 2 )
		dim = boost::lexical_cast<size_t>(argv[2]);
	
	std::uniform_real_distribution<double> distribution(-1,1);
	std::mt19937 engine;
	auto generator = [&]{return distribution(engine);};
	
	SquareMatrix m(dim);
	std::generate(m.begin(),m.end(),generator);


	std::vector<size_t> perThreadCount(THREADS,0);
	tbb::task_scheduler_init tsInit(THREADS);
	tbb::task_group tg;
	
	Timer t("Time count");
	
	for ( size_t iTask=0; iTask<THREADS; ++iTask )
	{
  	tg.run([=,&perThreadCount,&m]
				{
					size_t taskSize = dim/THREADS;
					size_t taskStart = iTask*taskSize;
					size_t taskEnd = std::min(taskStart+taskSize, dim);
          size_t count = 0;
					for (size_t i=taskStart; i<taskEnd; ++i)
					{
						for (size_t j=0; j<dim; ++j)
						{
            	if (m[i][j]>=0.0)
							{
							  ++perThreadCount[iTask];
								//++count;
							}
						}
					}
					// perThreadCount[iTask] = count;
				});
	}
	tg.wait();

	auto positiveCount = std::accumulate(perThreadCount.begin(),perThreadCount.end(),0);
	std::cout << dim << " by " << dim << " matrix has " << positiveCount << " positive entries = "
		<< 100.0*positiveCount/(dim*dim) << "%" << std::endl;

}

