#include <iostream>

class A
{
	public:
		A(int id) : m_id(id) 
		{
			std::cout << "An object of type A with ID " << m_id << " has been created\n";
		}                                                       
		
		~A() 
		{
			std::cout << "An object of type A with ID " << m_id << " has been destroyed\n";
		}                                                    

	private:
		int m_id;
};

int main(int argc, char* argv[])
{
	A a1(1);
	A a2(2);
}

