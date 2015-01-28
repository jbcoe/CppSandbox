// NO CLANG FORMAT (concepts not yet supported)
#include <iostream>
#include <type_traits>
#include <memory>

template <typename T>
concept bool Cloneable()
{
  return requires(const T& t)
  {
    { t.Clone() } -> std::unique_ptr<T>;
  };
}

struct MyClass 
{
  auto Clone() const { return std::make_unique<MyClass>(); }
};

template<typename T>
bool IsCloneable()
{
  return false;
}

template<Cloneable C>
bool IsCloneable()
{
  return true;
}

int main(int argc, char* argv[])
{
  std::cout << std::boolalpha << "IsCloneable" << " " << "MyClass" << " " << IsCloneable<MyClass>() << std::endl 
}

