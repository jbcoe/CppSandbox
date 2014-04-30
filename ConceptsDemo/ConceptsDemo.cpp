// NO CLANG FORMAT (concepts not yet supported)
#include <iostream>
#include <type_traits>

template <typename T>
concept bool Incrementable()
{
	return requires (T t) 
	{ 
		++t; 
	};
}

struct MyClass {};

struct MyIncrementableClass
{
  MyIncrementableClass& operator++() { return *this; }
};

void foo(Incrementable& i)
{
	std::cout << "foo" << std::endl;
}

int main(int argc, char* argv[])
{
	MyClass m;
	foo(m); // will not compile

	MyIncrementableClass im;
	foo(im);
}

