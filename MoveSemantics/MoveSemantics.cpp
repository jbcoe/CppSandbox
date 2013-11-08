#include <iostream>

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
    std::cout << " rvalue assignment" << std::endl;
    return *this;
  }

  static Moveable Instance()
  {
    std::cout << " Static function" << std::endl;
    return Moveable();
  }
};

struct S
{
  S(Moveable m_) : m(m_)
  {
  }
  Moveable m;
};

struct SS
{
  SS(Moveable m_) : m(std::move(m_))
  {
  }
  Moveable m;
};

int main()
{

  std::cout << "Moveable m" << std::endl;
  Moveable m;

  std::cout << "Moveable c(m)" << std::endl;
  Moveable c(m);

  std::cout << "m = c" << std::endl;
  m = c;

  std::cout << "m = Moveable::Instance()" << std::endl;
  m = Moveable::Instance();

  std::cout << "Moveable m2(Moveable::Instance())" << std::endl;
  Moveable m2(Moveable::Instance());

  std::cout << "Moveable m3 = std::move(m2)" << std::endl;
  Moveable m3 = std::move(m2);

  std::cout << "S s{Moveable{}}" << std::endl;
  S s{Moveable{}};

  std::cout << "SS ss{Moveable{}}" << std::endl;
  SS ss{Moveable{}};
}
