#include <iostream>

class MyType
{
	public:

		MyType(long double d) : m_d(d) {}

		long double value() const { return m_d; };
	
    MyType operator + (const MyType& m ) { return MyType(m_d+m.m_d); }

	private:

		long double m_d;

};

MyType operator "" _me (long double d)
{
	return MyType(d);
}

int main(int argc, char* argv[])
{
	auto myVar = ( 456.102_me + 32.03_me );
	std::cout << "Variable has value " << myVar.value() << std::endl;
}

