#include <iostream>
#include <algorithm>
#include <functional>

template <typename T>
struct Introspector
{
	static const std::function<int& (T*)> fPtrs[]; 

	static void RunFunction(T* t, std::function<void (int)> intFunction)
	{
		std::for_each(std::begin(fPtrs),std::end(fPtrs),
				[&](std::function<int& (T*)> f) { intFunction(f(t)); });
	}
};

struct CSomeData : Introspector<CSomeData>
{
	int m_age = 32;
	int m_number = 4;
	int m_prime = 5;
	
	void RunFunction(std::function<void (int)> intFunction)
	{
		static_assert(sizeof(CSomeData)==12,"3 ints");
		return Introspector<CSomeData>::RunFunction(this,intFunction);
	}
};

template<>
const std::function<int& (CSomeData*)> Introspector<CSomeData>::fPtrs[] = 
{ 
	&CSomeData::m_age, &CSomeData::m_number, &CSomeData::m_prime 
};

int main(int argc, char* argv[])
{
	CSomeData myData;
	auto lambda_printer = [](int theInt){std::cout << theInt << std::endl;};
	myData.RunFunction(lambda_printer);

	int total = 0;
	auto lambda_summer = [&total](int theInt){ total += theInt ;};
	myData.RunFunction(lambda_summer);

	std::cout << "The sum is " << total << std::endl;
}

