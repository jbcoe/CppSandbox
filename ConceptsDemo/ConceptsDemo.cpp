// NO CLANG FORMAT (concepts not yet supported)
#include <iostream>
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

void f(const auto& a)
{
  std::cout << "Generic f\n";
}

void f(const Cloneable& a)
{
  std::cout << "Cloneable f\n";
}

int main(int argc, char* argv[])
{
  f(MyClass());
}

