// Maximum subarray problem : Kadane's algorithm based on http://en.wikipedia.org/wiki/Maximum_subarray_problem
#include <iostream>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>
#include <type_traits>

template<typename Iterator_t>
std::pair<Iterator_t,	Iterator_t> max_subarray(const Iterator_t begin, const Iterator_t end)
{
	typedef typename std::remove_const<typename std::remove_reference<decltype(((Iterator_t*)nullptr)->operator*())>::type>::type Value_t;

	Value_t max_so_far{};
	Value_t max_ending_here{};
	auto it_max_begin = begin;
	auto it_max_begin_temp = begin;
	auto it_max_end = begin;

  for(auto it=begin, it_end=end; it!=it_end; ++it)
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
	return {it_max_begin,it_max_end};
}

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
	
	std::vector<int>::const_iterator it_max_begin;
	std::vector<int>::const_iterator it_max_end;

	std::tie(it_max_begin,it_max_end) = max_subarray(myInts.cbegin(),myInts.cend());

	std::copy(it_max_begin, it_max_end, osi);
  //std::cout << " [Values with largest consecutive sum [" 
	//					<< std::distance(myInts.cbegin(),it_max_begin) 
	//					<< ':' << std::distance(myInts.cbegin(),it_max_end) << ") ]" << std::endl;
}
