#include <iostream>
#include <stdlib.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

////////////////////////////////////////////////////////////

class MyClass
{
public:
  int m_iInt;
  double m_dbl;
  MyClass() {}

  static void DeAlloc(MyClass* pC) { pC->~MyClass(); }
};

////////////////////////////////////////////////////////////

class MyOtherClass
{
public:
  double m_dbl;
  int m_iInt;
  MyOtherClass() {}

  static void DeAlloc(MyOtherClass* pC) { pC->~MyOtherClass(); }
};

////////////////////////////////////////////////////////////

class MyBlock : boost::noncopyable
{
  void* m_pBlock;

public:
  MyBlock(int iSize) { m_pBlock = malloc(iSize); }

  ~MyBlock() { free(m_pBlock); }

  operator void*() { return m_pBlock; }
};

////////////////////////////////////////////////////////////

int main()
{
  boost::shared_ptr<MyBlock> pBlock(new MyBlock(sizeof(MyClass)));

  boost::shared_ptr<MyClass> pMyInstance(new ((void*)*pBlock) MyClass(),
                                         &MyClass::DeAlloc);

  pMyInstance->m_iInt = 8;
  pMyInstance->m_dbl = 9.3;

  boost::shared_ptr<MyOtherClass> pMySecondInstance(
      new ((void*)*pBlock) MyOtherClass(), &MyOtherClass::DeAlloc);

  std::cout << pMyInstance->m_dbl << std::endl;
  std::cout << pMyInstance->m_iInt << std::endl;

  std::cout << pMySecondInstance->m_dbl << std::endl;
  std::cout << pMySecondInstance->m_iInt << std::endl;
}

////////////////////////////////////////////////////////////
