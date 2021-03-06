#include <cassert>
#include <eggs/variant.hpp>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std::string_literals;

////////////////////////////////////////////////////////////////////////////////

template <typename F>
struct forwarder_t : private F
{
  using F::operator();

  using func = F;

  forwarder_t(F f) : F(std::move(f))
  {
  }
};

template <typename F>
struct forwarder_t<F&> : private F
{
  using F::operator();

  using func = F;

  forwarder_t(F f) : F(std::move(f))
  {
  }
};

template <typename T, typename... Ts>
struct forwarder_t<T (&)(Ts...)>
{

  using F = T (&)(Ts...);
  F f_;

  T operator()(Ts... ts) const
  {
    return f_(std::forward<Ts>(ts)...);
  }

  forwarder_t(F f) : f_(f)
  {
  }
};

template <typename T, typename... Ts>
struct overload_t : private forwarder_t<T>, private overload_t<Ts...>
{
  using forwarder_t<T>::operator();
  using overload_t<Ts...>::operator();

  using forwarder = forwarder_t<T>;

  overload_t(T t, Ts... ts)
      : forwarder_t<T>(std::move(t)), overload_t<Ts...>(std::forward<Ts>(ts)...)
  {
  }
};

template <typename T>
struct overload_t<T> : private forwarder_t<T>
{
  using forwarder_t<T>::operator();

  using forwarder = forwarder_t<T>;

  overload_t(T t) : forwarder_t<T>(std::move(t))
  {
  }
};

template <typename... Ts>
auto overload(Ts&&... ts)
{
  return overload_t<Ts...>(std::forward<Ts>(ts)...);
}

////////////////////////////////////////////////////////////////////////////////

#include <iostream>

void f(long l)
{
  std::cout << "long" << '\n';
}

struct s_t
{
  void operator()(float) const
  {
    std::cout << "float" << '\n';
  }
};

int main()
{
  struct {
    void operator()(uint8_t) const
    {
      std::cout << "uint8_t" << '\n';
    }
  } h_uint;

  s_t s;
  
  auto h_char = [](char) { std::cout << "char" << '\n'; };

  // check that all of the different sorts of function-like objects can be used
  auto o = overload(s, f, [](double) { std::cout << "double" << '\n'; }, h_char,
                    h_uint, [](auto) { std::cout << "other" << '\n'; },
                    [](auto, auto...) { std::cout << "others" << '\n'; });


  eggs::variant<double, float, char, uint8_t, long, std::string> v(uint8_t(0));
  eggs::variants::apply(o, v);
}

