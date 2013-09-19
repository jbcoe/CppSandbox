// Test.cpp

#include <iostream>

////////////////////////////////////////////

template <typename T, typename TInterface>
struct TSetImpl : public TInterface
{
  virtual void doThing()
  {
    std::cout << "CSetImpl" << std::endl;
  }
};

////////////////////////////////////////////

struct IPublic
{
  virtual void doThing() = 0;
};

////////////////////////////////////////////

struct CPublicImpl : public TSetImpl<int, IPublic>
{
};

////////////////////////////////////////////

int main()
{
  CPublicImpl p;
  p.doThing();
  return 0;
}

////////////////////////////////////////////

// ~Test.cpp
