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

		~Moveable() 
		{
			std::cout << "Destructor" << std::endl;
		}

		static Moveable Instance()
		{
			return Moveable();
		}
};

int main()
{
	Moveable m;
	Moveable c(m);
  Moveable m3(Moveable::Instance());
}
