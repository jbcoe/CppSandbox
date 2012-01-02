#include <boost/static_assert.hpp>
#include <iostream>

template <unsigned long N>
struct binary
{
	BOOST_STATIC_ASSERT(( N % 10 < 2));

	static unsigned const value = binary<N/10>::value * 2 + N%10;
};

template<>
struct binary<0>
{
	static unsigned const value = 0;
};

int main(int argc, const char* argv[])
{

#define PRINT_AS_BINARY_AND_VALUE(theValue) \
{\
	std::cout << theValue << " is the binary representation of " << binary<theValue>::value << std::endl;\
}

	PRINT_AS_BINARY_AND_VALUE(111);
	PRINT_AS_BINARY_AND_VALUE(1011);
	PRINT_AS_BINARY_AND_VALUE(1101);
	PRINT_AS_BINARY_AND_VALUE(101010);

#undef PRINT_AS_BINARY_AND_VALUE	

}
