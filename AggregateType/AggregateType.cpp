#include <iostream>
#include <memory>

template <typename T1, typename T2, typename T3>
class Aggregate
{
public:
  template <typename U>
  Aggregate(std::shared_ptr<const U> u) 
  {
      impl_ = std::make_shared<const InnerImpl<U>>(std::move(u));
  }

  template <typename V>
  V& As() const
  {
    return static_cast<V&>(*impl_);
  }

  template <typename V>
  std::shared_ptr<const V> AsPtr() const
  {
    return static_cast<std::shared_ptr<const V>>(*impl_);
  }

private:

  struct Inner
  {
    virtual operator const T1& () const = 0;  
    virtual operator const T2& () const = 0;  
    virtual operator const T3& () const = 0;  
    
    virtual operator std::shared_ptr<const T1> () const = 0;
    virtual operator std::shared_ptr<const T2> () const = 0;
    virtual operator std::shared_ptr<const T3> () const = 0;
  };

  std::shared_ptr<const Inner> impl_;
  
  template <typename U>
  struct InnerImpl : Inner
  {
    InnerImpl(std::shared_ptr<const U> u) : u_(std::move(u)) {}

    operator const T1& () const override { return *u_; }   
    operator const T2& () const override { return *u_; }  
    operator const T3& () const override { return *u_; }  
    
    operator std::shared_ptr<const T1> () const override { return u_; }   
    operator std::shared_ptr<const T2> () const override { return u_; }  
    operator std::shared_ptr<const T3> () const override { return u_; }  
    
    std::shared_ptr<const U> u_;
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

  Aggregate<Cat,Pig,Dog> agg(std::make_shared<const Chimera>());

  std::cout << agg.AsPtr<Cat>()->MakeNoise() << '\n';
  std::cout << agg.AsPtr<Pig>()->MakeNoise() << '\n';
  std::cout << agg.AsPtr<Dog>()->MakeNoise() << '\n';
}

