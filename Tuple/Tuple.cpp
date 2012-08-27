#include <iostream>
#include <vector>
#include <tuple>

int main(int argc, char* argv[])
{
	std::vector<double> v;
	v.push_back(2.718281);
  v.push_back(0.0);
  v.push_back(5.0);

  auto testTuple = std::make_tuple(0,3.14159,"The news",v);

	std::cout << "Tuple data: " << std::get<0>(testTuple) << std::endl; 
	std::cout << "Tuple data: " << std::get<1>(testTuple) << std::endl; 
	std::cout << "Tuple data: " << std::get<2>(testTuple) << std::endl; 
	for ( const auto& v : std::get<3>(testTuple) )
	{
		std::cout << "Tuple vector data: " << v << std::endl;
	}
}

