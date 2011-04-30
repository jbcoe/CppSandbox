#include <iostream>
#include <random>
#include <functional>

int main(int argc, char* argv[])
{
	std::uniform_int_distribution<int> distribution(0,99);
	std::mt19937 engine;
	auto generator = std::bind(distribution,engine);

	for ( int i=0; i<50; ++i )
	{
		int random = generator();
		std::cout << random << " ";
	}
	std::cout << std::endl;
}

