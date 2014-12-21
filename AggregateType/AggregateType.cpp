#include <iostream>
#include <memory>

template <typename T1, typename T2, typename T3>
class Aggregate
{
public:
  template <typename U>
  Aggregate(std::shared_ptr<U> u) 
  {
      impl_ = std::make_shared<InnerImpl<U>>(std::move(u));
  }

  template <typename V>
  V& As()
  {
    return static_cast<V&>(*impl_);
  }

  template <typename V>
  std::shared_ptr<V> AsPtr()
  {
    return static_cast<std::shared_ptr<V>>(*impl_);
  }

private:

  struct Inner
  {
    virtual operator T1& () = 0;  
    virtual operator T2& () = 0;  
    virtual operator T3& () = 0;  
    
    virtual operator std::shared_ptr<T1> () = 0;
    virtual operator std::shared_ptr<T2> () = 0;
    virtual operator std::shared_ptr<T3> () = 0;
  };

  std::shared_ptr<Inner> impl_;
  
  template <typename U>
  struct InnerImpl : Inner
  {
    InnerImpl(std::shared_ptr<U> u) : u_(std::move(u)) {}

    operator T1& () override { return *u_; }   
    operator T2& () override { return *u_; }  
    operator T3& () override { return *u_; }  
    
    operator std::shared_ptr<T1> () override { return u_; }   
    operator std::shared_ptr<T2> () override { return u_; }  
    operator std::shared_ptr<T3> () override { return u_; }  
    
    std::shared_ptr<U> u_;
  };
};

struct Cat
{
  const char* MakeNoise() const { return "Meeow"; }
}; 

struct Pig
{
  const char* MakeNoise() const { return "Oink"; }
}; 

struct Dog
{
  const char* MakeNoise() const { return "Woof"; }
};

struct Chimera : Cat, Pig, Dog {};

int main(int argc, char* argv[]) {

  Aggregate<Cat,Pig,Dog> agg(std::make_shared<Chimera>());

  std::cout << agg.AsPtr<Cat>()->MakeNoise() << '\n';
  std::cout << agg.AsPtr<Pig>()->MakeNoise() << '\n';
  std::cout << agg.AsPtr<Dog>()->MakeNoise() << '\n';
}

