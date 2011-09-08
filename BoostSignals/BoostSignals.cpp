#include <iostream>
#include <boost/signal.hpp>
#include <vector>
#include <numeric>

struct HelloWord
{                   
	void operator()() const
	{
		std::cout << "Hello World" << std::endl;
	}
};

template <typename T>
void print_all( T first, T last)
{                           
	if ( first == last )
		return; 
	while ( first != last )
		std::cout << *first++ << ' ';
	std::cout << std::endl;
}

template <typename T>
void print_sum( T first, T last)
{
	std::cout << std::accumulate(first,last,0);
}

int main(int argc, char* argv[])
{
	boost::signal<void ()> signal;
	HelloWord hello;
	signal.connect(hello);

	signal();

	boost::signal2<void,
			std::vector<double>::const_iterator,
			std::vector<double>::const_iterator > listSignal;

	std::vector<double> myList = {1.0, 2.0, 3.0};

  typedef std::vector<double>::const_iterator DVec_it;

	listSignal.connect(&print_all<std::vector<double>::const_iterator>);
	
	listSignal.connect(&print_sum<std::vector<double>::const_iterator>); 
  
	listSignal(myList.begin(),myList.end());
}

