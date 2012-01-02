#include <boost/static_assert.hpp>
#include <iostream>

template <unsigned long N>
struct binary
{
	BOOST_STATIC_ASSERT(( N % 10 < 2));

	static unsigned const value
		= binary<N/10>::value * 2
			+ N%10;
};

template<>
struct binary<0>
{
	static unsigned const value = 0;
};

int main(int argc, const char* argv[])
{
	unsigned const seven = binary<111>::value;
}
