#include <iostream>

class Moveable
{
	public:

		int m_iField;
	
		Moveable()
		{
			m_iField = 0;
			std::cout << "Default constructor" << std::endl;
		}

		Moveable(const Moveable& m)
		{
			m_iField = 1;
			std::cout << "Copy constructor" << std::endl;
		}

		Moveable(const Moveable&& m)
		{
			m_iField = 2;
			std::cout << "Move constructor" << std::endl;
		}

    Moveable& operator = (const Moveable& m)
		{
			m_iField = m.m_iField;
			std::cout << "normal assignement" << std::endl;
		}

    Moveable& operator = (const Moveable&& m)
		{
			m_iField = m.m_iField;
			std::cout << "rvalue assignement" << std::endl;
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
	Moveable c(m);
  Moveable m3;
	m3 = c;
}
