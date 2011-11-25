#include <iostream>

// A rather pointless function to illustrate a pointer to member function

template <typename Return_t, typename Class_t, typename Arg_t>
Return_t CallMemberFunction(Class_t* pInstance, 
		Return_t (Class_t::*pFn)(Arg_t), Arg_t arg)
{
	return (pInstance->*pFn)(arg);
}

class MyClass
{
public:
	
	MyClass(double multiplier) 
  : m_multiplier(multiplier)
	{
	}

	double Multiply(double value)
	{
		return value * m_multiplier;
	}

private:

	double m_multiplier;
};

int main(int argc, char* argv[])
{
	MyClass myDoubler(2.);

	std::cout << CallMemberFunction(&myDoubler,&MyClass::Multiply,2.) << std::endl;
}

