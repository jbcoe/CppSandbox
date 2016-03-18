#include <cassert>
#include <experimental/optional>
#include <iostream>

using std::experimental::optional;

template <typename... Ts>
class protocol
{
public:
  template <typename U>
  protocol(U&)
  {
  }
};

template <typename T, typename... Ts>
class protocol<T, Ts...> : public protocol<Ts...>
{
  T* const self_;

public:
  template <typename U,
            typename = std::enable_if_t<std::is_convertible<U&, T&>::value>>
  protocol(U& u) : protocol<Ts...>(u), self_(&static_cast<T&>(u))
  {
  }

  template <typename U,
            typename = std::enable_if_t<std::is_convertible<U&, T&>::value>>
  protocol(U& u, protocol<Ts...> c)
      : protocol<Ts...>(c), self_(&static_cast<T&>(u))
  {
  }

  operator T&()
  {
    return *self_;
  }
  
  T* ptr()
  {
    return self_;
  }
};

template <typename T, typename... Ts>
T& as(protocol<Ts...>& c)
{
  return static_cast<T&>(c);
}

template <typename T, typename... Ts>
optional<protocol<T, Ts...>> try_as(protocol<Ts...>& c)
{
  auto p = dynamic_cast<T*>(c.ptr());
  if (!p) return {};
  return protocol<T, Ts...>(*p, c);
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

struct PVAble
{
  virtual double PV() const = 0;
  virtual ~PVAble() = default;
};

struct DooDad : Cloneable, Named, Serializeable
{
  void Store() const override
  {
  }
  const char* Name() const override
  {
    return "Thingumy";
  }
  std::unique_ptr<Cloneable> Clone() const override
  {
    return std::make_unique<DooDad>(*this);
  }
};

void serializeNamedClone(protocol<Serializeable, Named, Cloneable> c)
{
  auto x = as<Cloneable>(c).Clone();
  const char* name = as<Named>(c).Name();
  as<Serializeable>(c).Store();
}

int main(int argc, char* argv[])
{
  DooDad d;
  serializeNamedClone(d);

  protocol<Named, Cloneable> nc(d);
  protocol<Cloneable, Named> cn(nc);

  assert(try_as<Serializeable>(nc));
  assert(!try_as<PVAble>(nc));
}

