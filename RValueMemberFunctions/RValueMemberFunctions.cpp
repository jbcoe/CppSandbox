// http://stackoverflow.com/questions/17521238/what-are-rvalue-references-for-this-for

#include <iostream>

class MyClass
{
public:
  void DoThing() const &
  {
    std::cout << "Thing was done as const &" << std::endl;
  }
  void DoThing() const &&
  {
    std::cout << "Thing was done as const &&" << std::endl;
  }

  void DoThing() &
  {
    std::cout << "Thing was done as &" << std::endl;
  }
  void DoThing() &&
  {
    std::cout << "Thing was done as &&" << std::endl;
  }
};

int main(int argc, char* argv[])
{
  MyClass m;
  m.DoThing();

  MyClass().DoThing();

  const MyClass& crm = m;
  crm.DoThing();

  auto returnMyClass = []()->const MyClass
  {
    return MyClass();
  }
  ;
  returnMyClass().DoThing();
}
