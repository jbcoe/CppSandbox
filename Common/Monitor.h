#include <thread>

template <typename T>
class Monitor
{
	T t;
	std::mutex m;
public:
	Monitor(T _t) : t(_t) {}
	Monitor(){}

	template <typename F>
	auto operator()(F f) -> decltype(f(t))
	{
		std::lock_guard<std::mutex> l(m);
		return f(t);
	}
};

