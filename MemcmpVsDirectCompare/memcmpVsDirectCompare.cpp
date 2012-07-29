#include <iostream>
#include <algorithm>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <Common/Timer.h>

typedef int COMPARED_TYPE;
static const COMPARED_TYPE COMPARED_VALUE = 1;

int main(int argc, char* argv[])
{                                                  
	if ( argc != 2 )
	{
		std::cerr << "Requires an integer argument" << std::endl;
		return -1;
	}

	size_t size = boost::lexical_cast<size_t>(argv[1]);
	int sum_of_cmps = 0; //Here to stop the optimizer stripping away code

	std::vector<COMPARED_TYPE> numbers(size,COMPARED_VALUE);
	std::vector<COMPARED_TYPE> moreNumbers(size,COMPARED_VALUE);

	{
		Timer t("memcmp");
		int cmp = memcmp(&numbers[0],&moreNumbers[0],size*sizeof(COMPARED_TYPE));
		sum_of_cmps += cmp;
	}

	{
		Timer t("explicit compare");
		int cmp = 0;
		for (size_t i=0; i<size; ++i)
		{
			if ( numbers[i] != moreNumbers[i] )
			{
				cmp = 1;
				break;
			}
		}
		sum_of_cmps += cmp;
	}

	{
		Timer t("std equals");
		int cmp = std::equal(numbers.begin(),numbers.end(),moreNumbers.begin());
		sum_of_cmps += cmp;
	}

	return sum_of_cmps==5;
}

