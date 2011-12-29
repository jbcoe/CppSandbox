#include <iostream>

template<int depth>
struct unary_recurser
{
	template <typename T>
	static T apply(T(*f)(T), T t)
	{
		static_assert(depth>0,"Recursion depth must be greater than 0");
		return unary_recurser<depth-1>::apply(f,f(t));
	}
};

template<>
struct unary_recurser <0>
{
	template <typename T>
	static T apply(T(*f)(T), T t)
	{
		return t;
	}
};

template <int multiple>
double multiplier(double value)
{
	return multiple * value;
}

int main(int argc, char* argv[])
{
	std::cout << unary_recurser<5>::apply(&multiplier<2>,1.) << std::endl;
}

