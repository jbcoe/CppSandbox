#include <iostream>
#include <vector>
#include <tuple>

typedef std::tuple <int,double,std::string,std::vector<double>> testTuple;

int main(int argc, char* argv[])
{
	testTuple tuple;
	auto& v0 = std::get<0>(tuple);
	auto& v1 = std::get<1>(tuple);
	auto& v2 = std::get<2>(tuple);
	auto& v3 = std::get<3>(tuple);
	
	v0 = 0;
	v1 = 3.14159;
	v2 = "The news";
  v3.push_back(2.718281);
  v3.push_back(0.0);
  v3.push_back(5.0);

	std::cout << "Tuple data: " << std::get<0>(tuple) << std::endl; 
	std::cout << "Tuple data: " << std::get<1>(tuple) << std::endl; 
	std::cout << "Tuple data: " << std::get<2>(tuple) << std::endl; 
	for ( const auto& v : std::get<3>(tuple) )
	{
		std::cout << "Tuple vector data: " << v << std::endl;
	}
}

