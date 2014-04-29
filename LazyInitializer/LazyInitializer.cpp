#include <iostream>
#include <type_traits>
#include <memory>
#include <boost/optional.hpp>

template <typename Initializer_t>
class Lazy
{
public:
  typedef decltype(std::declval<Initializer_t>()()) Value_t;

  Lazy(Initializer_t i) : m_i(std::move(i))
  {
  }

  Lazy(const Lazy<Initializer_t>& that) = delete;

  Lazy(Lazy<Initializer_t>&& that) = default;

  Lazy<Initializer_t>& operator=(const Lazy<Initializer_t>& that) = delete;

  Lazy<Initializer_t>& operator=(Lazy<Initializer_t>&& that) = default;

  operator Value_t&()
  {
    return get();
  }

  Value_t& get()
  {
    if (!m_v) m_v = m_i();
    return *m_v;
  }

private:
  Initializer_t m_i;
  boost::optional<Value_t> m_v;
};

template <typename Value_t, typename Initializer_t>
Lazy<Initializer_t> make_lazy(Initializer_t i)
{
  static_assert(
      std::is_same<decltype(std::declval<Initializer_t>()()), Value_t>::value,
      "Wrong type in initializer");
  return Lazy<Initializer_t>(std::move(i));
}

class MyClass
{
public:
  void Foo() const
  {
    std::cout << "Foo" << std::endl;
  }
};

int main(int argc, char* argv[])
{
  auto x = make_lazy<double>([]
  {
    std::cout << "Built variable" << std::endl;
    return 2.0;
  });

  std::cout << x << std::endl;
  std::cout << x << std::endl;
  std::cout << std::endl;
}
