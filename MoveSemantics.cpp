#include <iostream>

class Moveable
{
	public:

		Moveable()
		{
			std::cout << "Default constructor" << std::endl;
		}

		Moveable(const Moveable& m)
		{
			std::cout << "Copy constructor" << std::endl;
		}

		Moveable(const Moveable&& m)
		{
			std::cout << "Move constructor" << std::endl;
		}

    Moveable& operator = (const Moveable& m)
		{
			std::cout << "Normal assignement" << std::endl;
			return *this;
		}

    Moveable& operator = (const Moveable&& m)
		{
			std::cout << "rvalue assignement" << std::endl;
    	return *this;
		}

		static Moveable Instance()
		{
			std::cout << "Instance function" << std::endl;
			return Moveable();
		}
};

int main()
{
	
	Moveable m;
	
	std::cout << std::endl;
	
	Moveable c(m);
	
	std::cout << std::endl;

	m = c;
	
	std::cout << std::endl;
	
	m = Moveable::Instance();
	
	std::cout << std::endl;
	
	Moveable m2(Moveable::Instance());
}
