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
};

////////////////////////////////////////////////////////////

class MyOtherClass
{
public:
  double m_dbl;
  int m_iInt;
  MyOtherClass() {}
};

////////////////////////////////////////////////////////////

class MyBlock : boost::noncopyable
{
  void* m_pBlock;

public:
  MyBlock(int iSize) { m_pBlock = malloc(iSize); }

  ~MyBlock() { free(m_pBlock); }

  void* data() { return m_pBlock; }
};

////////////////////////////////////////////////////////////

struct Deleter
{
	template<typename T>
	void operator()(T* pT)
	{
		pT->~T();
	}
};

int main()
{
  MyBlock block(sizeof(MyClass));

  std::unique_ptr<MyClass, Deleter> pMyInstance(new (block.data()) MyClass());

  pMyInstance->m_iInt = 8;
  pMyInstance->m_dbl = 9.3;

	std::unique_ptr<MyOtherClass, Deleter> pMySecondInstance(new (block.data()) MyOtherClass());

  std::cout << pMyInstance->m_dbl << std::endl;
  std::cout << pMyInstance->m_iInt << std::endl;

  std::cout << pMySecondInstance->m_dbl << std::endl;
  std::cout << pMySecondInstance->m_iInt << std::endl;
}

////////////////////////////////////////////////////////////
