#include <iostream>
#include <algorithm>
#include <functional>

template <typename T>
struct HasFunctionList
{
	static const std::function<int& (T*)> fPtrs[]; 

	static void RunFunction(T* t, std::function<void (int)> intFunction)
	{
		std::for_each(std::begin(fPtrs),std::end(fPtrs),
				[&](std::function<int& (T*)> f)
				{
					intFunction(f(t));
				});
	}
};

struct CSomeData : HasFunctionList<CSomeData>
{
	int m_age = 32;
	int m_number = 4;
	int m_prime = 5;

	CSomeData()
	{
		static_assert(sizeof(CSomeData)==12,"3 ints");
	}
	
	void RunFunction(std::function<void (int)> intFunction)
	{
		return HasFunctionList<CSomeData>::RunFunction(this,intFunction);
	}
};

template<>
const std::function<int& (CSomeData*)> HasFunctionList<CSomeData>::fPtrs[] = 
{ 
	&CSomeData::m_age, 
	&CSomeData::m_number, 
	&CSomeData::m_prime 
};

int main(int argc, char* argv[])
{
	CSomeData myData;
	auto lambda_printer = [](int theInt){std::cout << theInt << std::endl;};
	myData.RunFunction(lambda_printer);
}

