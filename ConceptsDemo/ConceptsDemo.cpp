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

#define print_concept(x,y) std::cout << std::boolalpha << #x << " " << #y << " " << x<y>() << std::endl 

int main(int argc, char* argv[])
{
  print_concept(Cloneable,MyClass);
}

