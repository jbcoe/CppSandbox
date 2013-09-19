#include <iostream>

class CMyClass
{
public:
  CMyClass(int i) : data{i}
  {
  }
  std::string ClassName()
  {
    return "MyClass";
  }
  int DataValue()
  {
    return data;
  }

private:
  int data = 1;
};

int main(int argc, char* argv[])
{
  CMyClass* pObj = nullptr;
  std::cout << "Member function to get class name (requires no member data) "
            << pObj->ClassName() << std::endl;
  // std::cout << "Member function to get data (requires member data) " <<
  // pObj->DataValue() << std::endl; // Will crash
}
