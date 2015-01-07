// NO CLANG FORMAT (concepts not yet supported)
#include <iostream>
#include <type_traits>
#include <memory>
#include "origin/core/Concepts.hpp"
#include "origin/sequence/Concepts.hpp"

using namespace origin;

template <typename T>
concept bool Cloneable()
{
  return requires(T* t)
  {
    { t->Clone() } -> std::unique_ptr<T>;
  };
}

struct MyClass 
{
  std::unique_ptr<MyClass> Clone() { return std::unique_ptr<MyClass>(new MyClass()); }
};

struct MyNonCopyableClass
{
  MyNonCopyableClass(const MyNonCopyableClass&) = delete;
};

struct MyAdvanceableClass
{
  MyAdvanceableClass& operator++() { return *this; }
};

void foo(Advanceable& i)
{
  std::cout << "advanceable foo" << std::endl;
}

template<typename T>
void foo(T& i)
{
  std::cout << "default foo" << std::endl;
}

#define print_concept(x,y) std::cout << std::boolalpha << #x << " " << #y << " " << x<y>() << std::endl 

int main(int argc, char* argv[])
{
  MyClass m;
  foo(m);

  MyAdvanceableClass im;
  foo(im);

  print_concept(Copyable, MyClass);
  print_concept(Copyable, MyNonCopyableClass);

  std::cout << Cloneable<MyClass>() << std::endl;
}


