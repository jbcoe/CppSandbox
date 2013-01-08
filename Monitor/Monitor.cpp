// Based on Herb Sutter's talk at C++ and beyond 2012
// He made T t; mutable which I think is incorrect

#include <iostream>
#include <algorithm>
#include <string>
#include <thread>
#include <vector>

template <typename T>
class Monitor
{
	T t;
	std::mutex m;
public:
	Monitor(T _t) : t(_t) {}

	template <typename F>
	auto operator()(F f) -> decltype(f(t))
	{
		std::lock_guard<std::mutex> l(m);
		return f(t);
	}
};

int main(int argc, char* argv[])
{
	Monitor<std::ostream&> async_ios(std::ref(std::cout));
	
	std::vector<std::thread> threads;
	
	threads.emplace_back([&](){ 
			async_ios(
				[](std::ostream& io){ io << "Hello 1" << std::endl;}); 
			});
	threads.emplace_back([&](){ 
			async_ios(
				[](std::ostream& io){ io << "Hello 2" << std::endl;}); 
			});
	threads.emplace_back([&](){ 
			async_ios(
				[](std::ostream& io){ io << "Hello 3" << std::endl;}); 
			});
	threads.emplace_back([&](){ 
			async_ios(
				[](std::ostream& io){ io << "Hello 4" << std::endl;}); 
			});

	for(auto& t : threads) { t.join(); }
}

