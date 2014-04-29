#include <iostream>
#include <type_traits>
                     
class MyClass
{
};

template <typename T>
void foo(T&& t)
{
  static_assert( ! std::is_rvalue_reference<T&&>::value, "Do not use temporaries in foo!");
	std::cout << "foo" << std::endl;
}

int main(int argc, char* argv[])
{
	//foo(MyClass()); // triggers static assert

	MyClass m;
	foo(m);

	return 0;
}

