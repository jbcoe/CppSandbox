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

class B : public A
{
	public:
		B(int id) : A(id), m_id(id), m_a(id+1) 
		{
			std::cout << "An object of type B with ID " << m_id << " has been created\n";
		}                                                       
		
		~B() 
		{
			std::cout << "An object of type B with ID " << m_id << " has been destroyed\n";
		}                                                    

	private:
		int m_id;
		A m_a;
};

int main(int argc, char* argv[])
{
	A a1(1);
	A a2(2);
	B b(3);
}

