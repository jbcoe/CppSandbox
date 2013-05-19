#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>

template <typename Iterator_t, typename UnaryFunction_t, typename IfFunctor_t, typename ElseFunctor_t>
void for_each_if_else(Iterator_t begin, Iterator_t end, UnaryFunction_t condition, IfFunctor_t f, ElseFunctor_t g)
{
	while( begin != end )
	{
		auto&& v = *begin;

		if (condition(v)) f(v);
		else g(v);

		++begin;
	}
}

int main(int argc, char* argv[])
{           
	std::vector<int> ints(20);
	std::iota(ints.begin(), ints.end(), 1);

	for_each_if_else(ints.begin(), ints.end(), 
			[](int i){ return i%2 == 0; },
			[](int i){ std::cout << i << " is even" << std::endl; },
			[](int i){ std::cout << i << " is odd" << std::endl; } );
}
