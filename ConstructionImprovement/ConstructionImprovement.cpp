#include <iostream>

class MyClass
{
	private:

		double m_rValue = 3.14159;
		int m_iID = -1;

	public:
		MyClass(int iID) : m_iID(iID) 
	{                      
		std::cout << "Built and instance with ID: " << m_iID << std::endl; 
	}

		MyClass() : MyClass(42) 
	{
		std::cout << "Delegated construction " << m_iID << std::endl; 
		std::cout << "MyClass instance build with value : " 
			<< m_rValue << std::endl;
	}

		~MyClass()
		{
		}

};

int main(int argc, char* argv[])
{
	MyClass myInstance;
}

