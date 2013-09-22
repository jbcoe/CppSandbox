#include <iostream>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>

int main(int argc, char* argv[])
{                                                     
	std::random_device rd;
  std::uniform_int_distribution<int> distribution(-10,10);
  std::mt19937 engine(rd());
  auto generator = std::bind(distribution, engine);

	int count = 10;

	std::vector<int> myInts;
	myInts.reserve(count);

	std::generate_n(std::back_inserter(myInts),count,generator);

	std::vector<int> myPartialSum;
	std::partial_sum(begin(myInts), end(myInts), back_inserter(myPartialSum));

	std::vector<int> myReversePartialSum(myInts.size());
	std::partial_sum(myInts.rbegin(), myInts.rend(), myReversePartialSum.rbegin());

	
	std::ostream_iterator<int> osi(std::cout, " ");
	
	std::copy(begin(myInts), end(myInts), osi);
	std::cout << std::endl;
	
	std::copy(begin(myPartialSum), end(myPartialSum), osi);
	std::cout << std::endl;
	
	std::copy(myReversePartialSum.begin(), myReversePartialSum.end(), osi);
	std::cout << std::endl;

	auto end_offset = std::distance(begin(myPartialSum), max_element(begin(myPartialSum),end(myPartialSum)));
	auto begin_offset = std::distance(begin(myReversePartialSum), max_element(begin(myReversePartialSum),end(myReversePartialSum)));

	std::cout << begin_offset << " : " << end_offset << std::endl;

  if ( begin_offset > end_offset )
	{
		return 0;
	}

  auto begin_max_consecutive = begin(myInts) + begin_offset;
  auto end_max_consecutive = begin(myInts) + end_offset;

	std::copy(begin_max_consecutive, std::next(end_max_consecutive), osi);
	std::cout << std::endl;
}

