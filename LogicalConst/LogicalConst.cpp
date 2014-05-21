#include <iostream>
#include <memory>
template <typename T>
class logical_const
{
  typedef decltype(*std::declval<T>()) reference_type;

public:
  using value_type = typename std::enable_if<
      std::is_lvalue_reference<reference_type>::value,
      typename std::remove_reference<reference_type>::type>::type;

  ~logical_const() = default;

  logical_const() : t{} {}

  template <typename U>
  logical_const(U&& u)
      : t{std::forward<U>(u)}
  {
  }

  template <typename U>
  logical_const<T>& operator=(U&& u)
  {
    t = std::forward<U>(u);
    return *this;
  }

  template <typename U>
  logical_const(const logical_const<U>& pu)
      : t{pu.t}
  {
  }

  template <typename U>
  logical_const(logical_const<U>&& pu)
      : t{std::move(pu.t)}
  {
  }


  template <typename U>
  logical_const<T>& operator=(const logical_const<U>& pt)
  {
    t = pt.t;
    return *this;
  }

  template <typename U>
  logical_const<T>& operator=(logical_const<U>&& pt)
  {
    t = std::move(pt.t);
    return *this;
  }

  value_type* operator->() { return underlying_pointer(t); }

  const value_type* operator->() const { return underlying_pointer(t); }

  value_type* get() { return underlying_pointer(t); }

  const value_type* get() const { return underlying_pointer(t); }

  value_type& operator*() { return *t; }

  const value_type& operator*() const { return *t; }

  explicit operator bool() const { return static_cast<bool>(t); }

private:
  T t;

  template <typename U>
  static value_type* underlying_pointer(U* p)
  {
    return p;
  }

  template <typename U>
  static value_type* underlying_pointer(U& p)
  {
    return p.get();
  }

  template <typename U>
  static const value_type* underlying_pointer(const U* p)
  {
    return p;
  }

  template <typename U>
  static const value_type* underlying_pointer(const U& p)
  {
    return p.get();
  }
};

struct A
{
  void bar() const { std::cout << "bar (const)" << std::endl; }
  void bar() { std::cout << "bar (non-const)" << std::endl; }
};

struct B
{
  B() : m_ptrA(std::make_unique<A>()) {}

  void operator()() { m_ptrA->bar(); }

  void operator()() const { m_ptrA->bar(); }

  logical_const<std::unique_ptr<A>> m_ptrA;
  // std::unique_ptr<A> m_ptrA;
};

int main(int argc, char* argv[])
{
  const B b;
  b();

  logical_const<int*> pNull;
  std::cout << std::boolalpha << (pNull.get() == nullptr) << std::endl;
}
