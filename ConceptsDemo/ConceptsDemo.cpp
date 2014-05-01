// NO CLANG FORMAT (concepts not yet supported)
#include <iostream>
#include <type_traits>
#include "origin/core/Concepts.hpp"

using namespace origin;

template <typename T>
concept bool Incrementable()
{
	return requires (T t) 
	{ 
		++t; 
	};
}

struct MyClass {};
struct MyNonCopyableClass
{
	MyNonCopyableClass(const MyNonCopyableClass&) = delete;
};

struct MyIncrementableClass
{
  MyIncrementableClass& operator++() { return *this; }
};

void foo(Incrementable& i)
{
	std::cout << "foo" << std::endl;
}

#define print_concept(x,y) std::cout << std::boolalpha << #x << " " << #y << " " << x<y>() << std::endl 

int main(int argc, char* argv[])
{
	MyClass m;
	//foo(m); // will not compile

	MyIncrementableClass im;
	foo(im);

	print_concept(Copyable, MyClass);
	print_concept(Copyable, MyNonCopyableClass);
}


