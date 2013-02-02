// Based on Herb Sutter's talk at C++ and beyond 2012
// He made operator() const which I think is incorrect

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
	
	std::ostream* naughtyPointerOS = nullptr;

  async_ios([&](std::ostream& io){naughtyPointerOS = &io;});

	threads.emplace_back([&](){ *naughtyPointerOS << "Bad pointer lets me break the rules" << std::endl; }); 
	threads.emplace_back([&](){ *naughtyPointerOS << "Bad pointer lets me break the rules" << std::endl; }); 
	threads.emplace_back([&](){ *naughtyPointerOS << "Bad pointer lets me break the rules" << std::endl; }); 
	threads.emplace_back([&](){ 
			async_ios(
				[](std::ostream& io){ io << "Hello 1 : " << std::this_thread::get_id() << std::endl;}); 
			});
	threads.emplace_back([&](){ 
			async_ios(
				[](std::ostream& io){ io << "Hello 2 : " << std::this_thread::get_id() << std::endl;}); 
			});
	threads.emplace_back([&](){ 
			async_ios(
				[](std::ostream& io){ io << "Hello 3 : " << std::this_thread::get_id() << std::endl;}); 
			});
	threads.emplace_back([&](){ 
			async_ios(
				[](std::ostream& io){ io << "Hello 4 : " << std::this_thread::get_id() << std::endl;}); 
			});

	for(auto& t : threads) { t.join(); }
}

