#include <iostream>

template <typename T, typename ...Ts>
struct Chimera : Chimera<Ts...>
{
  T* self_;
  template <typename U>
  Chimera(U& u) : Chimera<Ts...>(u), self_(&static_cast<T&>(u))
  {
  }
  explicit operator T&()
  {
    return *self_;
  }
};

template <typename T>
struct Chimera <T> 
{
  T* self_;
  template <typename U>
  Chimera(U& u) : self_(&static_cast<T&>(u))
  {
  }
  explicit operator T&()
  {
    return *self_;
  }
};

template <typename T, typename ...Ts>
T& as(Chimera<Ts...>& c)
{
  return static_cast<T&>(c);
}

struct Cloneable
{
  virtual std::unique_ptr<Cloneable> Clone() const = 0;
  virtual ~Cloneable() = default;
};

struct Serializeable
{                              
  virtual void Store() const = 0;
  virtual ~Serializeable() = default;
};

struct Named
{
  virtual const char* Name() const = 0;
  virtual ~Named() = default;
};

struct DooDad : Cloneable, Named, Serializeable {
  void Store() const override {}
  const char* Name() const override { return "Thingumy"; }
  std::unique_ptr<Cloneable> Clone() const override { return std::make_unique<DooDad>(*this); }
};

void serializeNamedClone(Chimera<Serializeable, Named, Cloneable> c)
{
  auto x = as<Cloneable>(c).Clone();
  const char* name = as<Named>(c).Name();
  as<Serializeable>(c).Store();
}

int main(int argc, char* argv[])
{
  DooDad d;
  serializeNamedClone(d);

  Chimera<Named, Cloneable> nc(d);
  Chimera<Cloneable, Named> cn(nc);
}

