#include <iostream>
#include <memory>

template <typename T>
class logical_const
{
public:
  typedef std::remove_reference_t<decltype(*std::declval<T>())> value_type;

  ~logical_const()
  {
  }

  logical_const() : t{}
  {
  }

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

  value_type* operator->()
  {
    return std::addressof(*t);
  }

  const value_type* operator->() const
  {
    return std::addressof(*t);
  }

  value_type* get()
  {
    return std::addressof(*t);
  }

  const value_type* get() const
  {
    return std::addressof(*t);
  }

  value_type& operator*()
  {
    return *t;
  }

  const value_type& operator*() const
  {
    return *t;
  }

  operator bool() const
  {
    return t;
  }

private:
  T t;
};

struct A
{
  void bar() const
  {
    std::cout << "bar (const)" << std::endl;
  }
  void bar()
  {
    std::cout << "bar (non-const)" << std::endl;
  }
};

struct B
{
  B() : m_ptrA(std::make_unique<A>())
  {
  }

  void operator()()
  {
    m_ptrA->bar();
  }

  void operator()() const
  {
    m_ptrA->bar();
  }

  logical_const<std::unique_ptr<A>> m_ptrA;
  // std::unique_ptr<A> m_ptrA;
};

int main(int argc, char* argv[])
{
  const B b;
  b();
}
