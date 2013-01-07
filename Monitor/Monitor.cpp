// Based on Herb Sutter's talk at C++ and beyond 2012
// He made T t; mutable which I think is incorrect

#include <iostream>
#include <string>
#include <thread>

template <typename T>
class Monitor
{
	T t;
	mutable std::mutex m;
public:
	Monitor(const T& _t) : t(_t) {}
	Monitor(T&& _t) : t(std::move(_t)) {}

	template <typename F>
	auto operator() (F f) const -> decltype(f(t))
	{
		std::lock_guard<std::mutex> l(m);
		return f(t);
	}
	
	template <typename F>
	auto operator() (F f) -> decltype(f(t))
	{
		std::lock_guard<std::mutex> l(m);
		return f(t);
	}
};

int main(int argc, char* argv[])
{
	Monitor<std::string> s("Locked hello world");
	s([](std::string& s){ s += " with Monitors"; });
	s([](std::string& s){ std::cout << s << std::endl; });
	
  
	const Monitor<std::string> cs("const Locked hello world");
	//cs([](std::string& s){ s += " with Monitors"; });  Won't compile as we can't call non const functions on a const Monitor
	cs([](const std::string& s){ std::cout << s << " with Monitors" << std::endl; });
}

