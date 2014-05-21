#include <iostream>

class MyClass
{
public:
  double m_rValue = 3.14159;
  int m_iID = -1;

  MyClass(int iID) : m_iID(iID)
  {
    std::cout << "Built instance with ID: " << m_iID << std::endl;
  }

  MyClass() : MyClass(42)
  {
    std::cout << "Delegated construction " << m_iID << std::endl;
    std::cout << "MyClass instance build with value : " << m_rValue
              << std::endl;
  }

  ~MyClass() {}

  static constexpr int s_iFive = 5;
};

int main(int argc, char* argv[])
{
  MyClass myInstance;
  std::cout << "\nmyInstance.m_rValue = " << myInstance.m_rValue << std::endl;
  std::cout << "myInstance.m_iID = " << myInstance.m_iID << std::endl;
  std::cout << "MyClass.s_iFive = " << MyClass::s_iFive << std::endl;
}
