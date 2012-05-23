#include <iostream>
#include <algorithm>
#include <functional>

struct CSomeData
{
	int m_age = 32;
	int m_number = 4;
	int m_prime = 5;

	static const std::function<int& (CSomeData*)> fPtrs[]; 
};

const std::function<int& (CSomeData*)> CSomeData::fPtrs[] = 
{ 
	&CSomeData::m_age, 
	&CSomeData::m_number, 
	&CSomeData::m_prime 
};

int main(int argc, char* argv[])
{
	CSomeData myData;
	std::for_each(std::begin(CSomeData::fPtrs),std::end(CSomeData::fPtrs),
			[&](std::function<int& (CSomeData*)> f)
			{
				std::cout << f(&myData) << std::endl;
			});
}

