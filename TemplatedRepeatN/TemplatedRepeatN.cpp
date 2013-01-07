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
	
	template <typename F>
	static void DoWithN(F f)
	{
		f(depth);
		Repeat<depth-1>::DoWithN(f);
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
	
	template <typename F>
	static void DoWithN(F f)
	{
		return;
	}
};


int main(int argc, char* argv[])
{              
	Repeat<5>::Do([]{std::cout << "Hello" << std::endl; });
	Repeat<5>::DoWithN([](int N){std::cout << "Goodbye " << N << std::endl; });
}

