#include <iostream>
#include <vector>
#include <atomic>
#include <random>
#include <thread>

std::atomic<long> generatorCount{0};

class ThreadLocalGenerator
{
	std::function<int (void)> f_;

	public:
  ThreadLocalGenerator()
	{
		std::mt19937 engine(generatorCount++);
		std::uniform_int_distribution<int> distribution(-5,5);
		f_ = std::bind(distribution, engine);
	}

	int operator()()
	{ 
		return f_(); 
	}
};

thread_local ThreadLocalGenerator generator{};

int main(int argc, char* argv[])
{
  int BIG_COUNT = 10000;

	std::vector<std::thread> generators;
	for(int i=0; i<BIG_COUNT; ++i)
	{
		generators.push_back(std::thread([&] 
		{ 
			int local_total = 0;
			{ 
				if ( generator() == 42 )
					std::cout << "Oh no!" << std::endl; //Cannot be hit
			} 
		}));
	}

	for(auto& t : generators) t.join();

  return 0;
}

