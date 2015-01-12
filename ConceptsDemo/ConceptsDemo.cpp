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
  return requires(const T& t)
  {
    { t.Clone() } -> std::unique_ptr<T>;
  };
}

template<typename T, typename Archive>
concept bool Serializeable()
{
  return requires(const T& t, Archive& a)
  {
    t.Store(a);
  } 
  && requires(T& t, const Archive& a)
  {
    t.Load(a);
  };
}

class PolymorphicArchive
{
};

struct MyClass 
{
  auto Clone() const { return std::make_unique<MyClass>(); }
  void Store(PolymorphicArchive& a) const {}
  void Load(const PolymorphicArchive& a) {}
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
  std::cout << Serializeable<MyClass,PolymorphicArchive>() << std::endl;
}


