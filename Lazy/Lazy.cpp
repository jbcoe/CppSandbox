#include <iostream>
#include <experimental/optional>

using std::experimental::optional;
template <typename T>
struct NullableValue
{
  typedef optional<T> value;
};

template <typename T>
struct NullableValue<T*>
{
};

template <typename T>
struct NullableValue<std::unique_ptr<T>>
{
  typedef std::unique_ptr<T> value;
};

template <typename T>
struct NullableValue<std::shared_ptr<T>>
{
  typedef std::shared_ptr<T> value;
};

template <typename F,
          typename Value_T =
              std::remove_reference_t<decltype(std::declval<F>()())>>
class Lazy
{
  typedef typename NullableValue<Value_T>::value NullableValue_T;
  NullableValue_T m_value;
  F m_f;

public:
  Lazy(F f) : m_f(std::move(f)) {}
  auto& operator*()
  {
    init();
    return *m_value;
  }
  auto* operator -> ()
  {
    init();
    return &*m_value;
  }
  auto* get()
  {
    init();
    return &*m_value;
  }

private:
  void init()
  {
    if (!m_value)
    {
      m_value = m_f();
    }
  }
};

template <typename F>
auto make_lazy(F f)
{
  return Lazy<F>(std::move(f));
}

class MyClass
{
  int m_x = 0;

public:
  MyClass(int x) : m_x(x) {}
  int GetValue() { return m_x; }
};

int main(int argc, char* argv[])
{
  auto lX = make_lazy([]
                      {
                        std::cout << "Building" << std::endl;
                        return 5.9;
                      });

  std::cout << *lX << std::endl;
  std::cout << *lX << std::endl;
  std::cout << *lX << std::endl;
  std::cout << *lX << std::endl;
  std::cout << *lX << std::endl;
  std::cout << std::endl;

  auto lptrX = make_lazy([]
                         {
                           std::cout << "Building" << std::endl;
                           return std::make_unique<MyClass>(5);
                         });

  std::cout << lptrX->GetValue() << std::endl;
  std::cout << lptrX->GetValue() << std::endl;
  std::cout << lptrX->GetValue() << std::endl;
  std::cout << lptrX->GetValue() << std::endl;
  std::cout << std::endl;

  auto f = [](MyClass* pM)
  {
    std::cout << "Called f" << std::endl;
  };

  f(lptrX.get());
}
