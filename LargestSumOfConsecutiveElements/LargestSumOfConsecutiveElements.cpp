// Maximum subarray problem : Kadane's algorithm based on http://en.wikipedia.org/wiki/Maximum_subarray_problem
#include <iostream>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>

int main(int argc, char* argv[])
{
  std::random_device rd;
  std::uniform_int_distribution<int> distribution(-10, 10);
  std::mt19937 engine(rd());
  auto generator = std::bind(distribution, engine);

  int count = 10;

  std::vector<int> myInts;// = {-7, -7, 8, 9, -1, -7, -5, 1, -7, 5};
  myInts.reserve(count);

  std::generate_n(std::back_inserter(myInts), count, generator);

  std::ostream_iterator<int> osi(std::cout, " ");
	
	std::copy(begin(myInts), end(myInts), osi);
  std::cout << " [Values]" << std::endl;
  
	auto max_so_far = 0;
	auto max_ending_here = 0;
	auto it_max_begin = myInts.cbegin();
	auto it_max_begin_temp = myInts.cbegin();
	auto it_max_end = myInts.cbegin();

  for(auto it=myInts.cbegin(), it_end=myInts.cend(); it!=it_end; ++it)
	{
		if ( max_ending_here < 0 )
		{
			max_ending_here = *it;
			it_max_begin_temp = it;
		}
		else
		{
			max_ending_here += *it;
		}
    
		if (max_ending_here>=max_so_far )
		{
			max_so_far = max_ending_here;
			it_max_begin = it_max_begin_temp;
			it_max_end = std::next(it);
		}
	}
	
	std::copy(it_max_begin, it_max_end, osi);
  std::cout << " [Values with largest consecutive sum [" 
						<< std::distance(myInts.cbegin(),it_max_begin) 
						<< ':' << std::distance(myInts.cbegin(),it_max_end) << ") ]" << std::endl;
}
