#include <iostream>

class MyClass
{
private:

	int m_iID = -1;

public:
	MyClass(int iID) : m_iID(iID) 
	{                      
		std::cout << "Built and instance with ID: " << m_iID << std::endl; 
	}

	MyClass() : MyClass(42) 
	{
		std::cout << "Delegated construction" << m_iID << std::endl; 
	}

	~MyClass()
	{
	}

};

int main(int argc, char* argv[])
{
	MyClass myInstance;
}

