#include <iostream>
#include <mutex>
#include <experimental/optional>

using std::experimental::optional;

template <typename Value_T>
class Lazy
{
  mutable optional<Value_T> m_value;
  mutable std::function<Value_T()> m_f;
  std::unique_ptr<std::mutex> m_mutex;

public:
  
  template<typename F>
  Lazy(F f) : m_f(std::move(f)), m_mutex(std::make_unique<std::mutex>()) {}

  Lazy(const Lazy&) = delete;
  Lazy& operator = (const Lazy&) = delete;

  Lazy(Lazy&&) = default;
  Lazy& operator = (Lazy&&) = default;
  
  auto& operator*()
  {
    init();
    return *m_value;
  }
  
  const auto& operator*() const
  {
    init();
    return *m_value;
  }
 
  auto* operator->()
  {
    init();
    return std::addressof(*m_value);
  }
  
  const auto* operator->() const
  {
    init();
    return std::addressof(*m_value);
  }

private:
  void init() const
  {
    std::lock_guard<std::mutex> l(*m_mutex);
    if ( ! m_value ) { m_value = m_f(); }
  }
};

template <typename F, typename T=decltype(std::declval<F&>()())>
auto make_lazy(F f)
{
  return Lazy<T>(std::move(f));
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

  auto lClass = make_lazy([]
                         {
                           std::cout << "Building" << std::endl;
                           return MyClass(5);
                         });

  std::cout << lClass->GetValue() << std::endl;
  std::cout << lClass->GetValue() << std::endl;
  std::cout << lClass->GetValue() << std::endl;
  std::cout << lClass->GetValue() << std::endl;
  std::cout << std::endl;

  auto f = [](MyClass& pM)
  {
    std::cout << "Called f" << std::endl;
  };

  f(*lClass);
}
