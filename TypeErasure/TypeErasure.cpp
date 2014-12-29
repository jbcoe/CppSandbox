#include <iostream>
#include <vector>

class Noisy
{
  public:

    template<typename T>
    Noisy(const T& t)
    {
      impl_ = std::make_shared<const InnerImpl<T>>(t);
    }

    const char* Noise() const
    {
      return impl_->Noise();
    }
  
  private:   

    struct Inner
    {
      virtual const char* Noise() const = 0;
    };
  
    std::shared_ptr<const Inner> impl_;

    template<typename ErasedType>
    struct InnerImpl : Inner
    {
      InnerImpl(const ErasedType& t) : t_(t) {}

      const char* Noise() const override
      {
        return t_.Noise();
      }

      ErasedType t_;
    };
};

struct Duck
{
  const char* Noise() const { return "Quack"; }
};

struct Cat
{
  const char* Noise() const { return "Meoow"; }
};

struct Cow
{
  const char* Noise() const { return "Moo"; }
};

int main(int argc, char* argv[])
{
  std::vector<Noisy> beasts;
  beasts.push_back(Duck());
  beasts.push_back(Cat());
  beasts.push_back(Cow());

  for(const auto& n : beasts)
  {
    std::cout << n.Noise() << '\n';
  }
}

