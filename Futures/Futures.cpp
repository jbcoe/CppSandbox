#include <iostream>
#include <future>

int evaluateSum (int a, int b)
{
	return a + b;
}

int main(int argc, char* argv[])
{
	std::future<int> result = std::async(evaluateSum,2,3);
	std::cout << "2 + 3 = " << result.get() << std::endl;
}

