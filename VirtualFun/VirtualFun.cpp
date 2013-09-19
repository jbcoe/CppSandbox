#include <iostream>

struct A
{
  void f()
  {
    std::cout << "A::f" << std::endl;
  }
};

struct B : A
{
  virtual void f()
  {
    std::cout << "B::f" << std::endl;
  }
};

struct C : B
{
  void f()
  {
    std::cout << "C::f" << std::endl;
  }
};

struct D : C
{
  void f()
  {
    std::cout << "D::f" << std::endl;
  }
};

int main(int argc, char* argv[])
{
  D d;

  A& aRd = d;
  aRd.f();

  B& bRd = d;
  bRd.f();

  C& cRd = d;
  cRd.f();
}
