#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
#include <random>
#include <Common/Timer.h>


	template <typename Inserter_t>
void RandomFill(Inserter_t inserter, size_t count, const size_t max)
{
	std::uniform_int_distribution<int> distribution(0,99);
	std::mt19937 engine;
	auto generator = std::bind(distribution,engine);

	std::generate_n(inserter,count,generator);
}

int main(int argc, char* argv[])
{
	size_t numbers{100000};
	size_t repeats{1000};
	
	size_t vector_removed5s{};
	{
		std::vector<int> ints{numbers};
		RandomFill(std::back_inserter(ints), numbers, 10);

		auto t = make_timer("vector");

		for (size_t i{}; i<repeats; ++i )
			vector_removed5s += std::distance(std::remove_if(ints.begin(),ints.end(), [](int v){return v == 5;}), ints.end());
	}

	size_t list_removed5s{};
	{
		std::list<int> ints{numbers};
		RandomFill(std::back_inserter(ints), numbers, 10);
		auto t = make_timer("list");

		for (size_t i{}; i<repeats; ++i )
			list_removed5s += std::distance(std::remove_if(ints.begin(),ints.end(), [](int v){return v == 5;}), ints.end());
	}

	return !(vector_removed5s == list_removed5s);
}

