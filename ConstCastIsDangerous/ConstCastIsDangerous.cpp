#include <iostream>

class MyClass
{
	public:

		MyClass(int value) : m_fixedValue(value) {} 

		const int m_fixedValue;

		static const int m_fixedStaticValue;
};

const int MyClass::m_fixedStaticValue = 9;

int main()
{
	const MyClass myInstance(9);
	std::cout << myInstance.m_fixedValue << std::endl;
	const_cast<int&>(myInstance.m_fixedValue) = 7;
	std::cout << myInstance.m_fixedValue << std::endl;
	const_cast<int&>(myInstance.m_fixedStaticValue) = 7;
	std::cout << MyClass::m_fixedStaticValue<< std::endl;
}

