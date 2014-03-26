#include <iostream>
#include <vector>

class Moveable
{
public:
  Moveable()
  {
    std::cout << " Default constructor" << std::endl;
  }

  Moveable(const Moveable& m)
  {
    std::cout << " Copy constructor" << std::endl;
  }

  Moveable(const Moveable&& m)
  {
    std::cout << " Move constructor" << std::endl;
  }

  Moveable& operator=(const Moveable& m)
  {
    std::cout << " Normal assignment" << std::endl;
    return *this;
  }

  Moveable& operator=(const Moveable&& m)
  {
    std::cout << " Move assignment" << std::endl;
    return *this;
  }

  static Moveable Instance()
  {
    std::cout << " Static function" << std::endl;
    return Moveable();
  }
};

int main()
{
  std::cout << "Moveable m" << std::endl;
  Moveable m;

	std::cout << std::endl;
  std::cout << "Moveable c(m)" << std::endl;
  Moveable c(m);

	std::cout << std::endl;
  std::cout << "m = c" << std::endl;
  m = c;

	std::cout << std::endl;
  std::cout << "m = Moveable::Instance()" << std::endl;
  m = Moveable::Instance();

	std::cout << std::endl;
  std::cout << "Moveable m2(Moveable::Instance())" << std::endl;
  Moveable m2(Moveable::Instance());

	std::cout << std::endl;
  std::cout << "Moveable m3 = std::move(m2)" << std::endl;
  Moveable m3 = std::move(m2);

	std::cout << std::endl;
	std::cout << "std::vector<Moveable> vms(4)" << std::endl;
	std::vector<Moveable> vms(4);

	std::cout << std::endl;
	std::cout << "vms.push_back(Moveable::Instance())" << std::endl;
	vms.push_back(Moveable::Instance());
}
