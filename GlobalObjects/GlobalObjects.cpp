#include <iostream>
#include <memory>
#include <string>

class A
{
public:
  A(std::string s) : m_name(std::move(s))
  {
  }

  const std::string& GetName()
  {
    return m_name;
  }

private:
  std::string m_name;
};

class B
{
public:
  static std::unique_ptr<A> g_ptrA;
};

std::unique_ptr<A> B::g_ptrA = std::unique_ptr<A>(new A("Global A"));

int main(int argc, const char* argv[])
{
  std::cout << B::g_ptrA->GetName() << std::endl;
  return 0;
}
