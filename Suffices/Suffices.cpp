#include <iostream>

class MyType;

MyType operator "" _me (long double d);

class MyType
{
	public:

		friend MyType operator "" _me (long double d);

	private:

		MyType(long double d) : m_d(d) {}

		long double m_d;

};

MyType operator "" _me (long double d)
{
	return MyType(d);
}

int main(int argc, char* argv[])
{
	auto myVar = 456.0_me;
}

