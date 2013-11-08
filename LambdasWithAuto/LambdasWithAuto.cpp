#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <random>

template <typename T>
constexpr T Zero = T(0);

std::uniform_real_distribution<double> distribution(0, 1);
std::mt19937 engine;
auto generator = std::bind(distribution, engine);

auto lambda_sum_sq = [](auto sum, auto x) { return sum + x*x; };

int main(int argc, char* argv[])
{                  
	std::vector<double> numbers;
	std::generate_n(back_inserter(numbers), 100, generator);
	
	auto sum2 = std::accumulate(numbers.begin(), numbers.end(), 
			Zero<decltype(numbers)::value_type>, lambda_sum_sq);
	
	std::cout << sum2 << std::endl; 
}

