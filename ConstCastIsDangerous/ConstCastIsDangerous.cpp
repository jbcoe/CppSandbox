#include <iostream>

class MyClass
{
	public:

		MyClass(int value) : m_fixedValue(value), m_nonFixedValue(value) {} 

		int m_nonFixedValue;

		const int m_fixedValue;

		static const int m_fixedStaticValue; 

		void printMe() const
		{
			std::cout << "  non fixed value: " << m_nonFixedValue << std::endl;
			std::cout << "  fixed value: " << m_fixedValue << std::endl;
			std::cout << "  static fixed value: " << m_fixedStaticValue << std::endl;
			std::cout << std::endl;
		}
};

const int MyClass::m_fixedStaticValue = 9;

int main()
{
	const MyClass myInstance(9);

	// Any of the following const-cast and set operations can cause a crash

	std::cout << "const_cast on member variable of const object " << std::endl;
	const_cast<int&>(myInstance.m_nonFixedValue) = 7;
	myInstance.printMe();
	
	std::cout << "const_cast on const member variable of const object " << std::endl;
	const_cast<int&>(myInstance.m_fixedValue) = 7;
	myInstance.printMe();
	
	std::cout << "const_cast on const static class variable" << std::endl;
	const_cast<int&>(MyClass::m_fixedStaticValue) = 7;
	myInstance.printMe();
}

