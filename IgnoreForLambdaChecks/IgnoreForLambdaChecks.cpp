#include <iostream>
#include <functional>
#include <tuple>

template <typename T>
void RunFunction(T t, int i)
{
	std::ignore = std::function<std::string(int)>(t);
	std::cout << t(i) << std::endl; 
}

int main(int argc, char* argv[])
{               
	RunFunction([](int x) { return "std::string (int)"; }, 5);
	// RunFunction([](int x) { return 4.0; }, 5);  assignment to std::ignore prevents this compiling
}

