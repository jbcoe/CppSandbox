#include <iostream>
#include <functional>

template<int depth>
struct Repeat
{
	template <typename F>
	static void Do(F f)
	{
		f();
		Repeat<depth-1>::Do(f);
	}
};

template<>
struct Repeat <0>
{
	template <typename F>
	static void Do(F f)
	{
		return;
	}
};


int main(int argc, char* argv[])
{              
	Repeat<5>::Do([]{std::cout << "Hello" << std::endl; });
}

