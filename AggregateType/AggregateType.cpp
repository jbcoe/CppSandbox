#include <iostream>
#include <memory>

template <typename T, typename... Ts>
class AggregateImpl : public AggregateImpl<Ts...>
{
public:
  template <typename U>
  AggregateImpl(std::shared_ptr<U> u)
      : AggregateImpl<Ts...>(std::move(u))
  {
  }

  struct Inner : public AggregateImpl<Ts...>::Inner
  {
    virtual operator T&() = 0;
  };
};

template <typename T>
class AggregateImpl<T>
{
public:
  template <typename U>
  AggregateImpl(std::shared_ptr<U> u)
  {
  }

  struct Inner
  {
    virtual operator T&() = 0;
  };

};

class Cat{}; class Pig{}; class Dog{};

struct Chimera : Cat, Pig, Dog {};

int main(int argc, char* argv[]) {

  AggregateImpl<Cat,Pig,Dog> agg(std::make_shared<Chimera>());
}



