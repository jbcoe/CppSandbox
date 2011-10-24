#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>

template<typename T>
void PrintCollection(T begin, T end)
{
	while(begin!=end)
	{
		std::cout << *begin++;
		if(begin!=end)
		std::cout << ' ';
	}
	std::cout << std::endl;
}
  
template<typename T>
void PrintCollection(T& collection)
{
	PrintCollection(collection.begin(), collection.end());
}

int main(int argc, char* argv[])
{
	std::vector<double> vrDoubles(15);
	std::fill(vrDoubles.begin(),vrDoubles.end(),0.0);

  auto it_1 = vrDoubles.begin();
  auto it_2 = vrDoubles.begin();

	advance(it_1, 5);
	advance(it_2, 10);

	assert(std::distance(it_1,it_2)==5);

  /////////////////////////////////////////////////////////
	
	std::vector<double> vrMoreDoubles;

	std::for_each(vrDoubles.begin(),vrDoubles.end(),
			[](double& rValue)->double
			{
				static double generatedValue = 1.0;
				generatedValue *= 2.0;
				rValue = generatedValue;
			});
	
	PrintCollection(vrDoubles);

	std::copy(vrDoubles.begin(),vrDoubles.end(),std::back_inserter(vrMoreDoubles));

	PrintCollection(vrMoreDoubles);

	std::stringstream ss;
	std::ostream_iterator<std::stringstream::char_type> os(ss);

	std::copy(vrDoubles.begin(),vrDoubles.end(),os);

}                             

