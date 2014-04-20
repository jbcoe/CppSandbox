#include <iostream>
#include <memory>

template <typename T>
class logical_const
{
public:
  typedef std::remove_reference_t<decltype(*std::declval<T>())> Value_T;

  ~logical_const()
  {
  }

  logical_const()
  {
  }

  logical_const(const T& t_) : t{t_}
  {
  }

  logical_const(T&& t_) : t{std::move(t_)}
  {
  }

  logical_const(const logical_const<T>& pt_) : t{pt_.t}
  {
  }

  logical_const(logical_const<T>&& pt_) : t{std::move(pt_.t)}
  {
  }

  logical_const<T>& operator=(const T& t_)
  {
    t = t_;
    return *this;
  }

  logical_const<T>& operator=(T&& t_)
  {
    t = std::move(t_);
    return *this;
  }

  logical_const<T>& operator=(const logical_const<T>& pt_)
  {
    t = pt_.t;
    return *this;
  }

  logical_const<T>& operator=(logical_const<T>&& pt_)
  {
    t = std::move(pt_.t);
    return *this;
  }

  Value_T* operator->()
  {
    return &*t;
  }

  const Value_T* operator->() const
  {
    return &*t;
  }

  Value_T& operator*()
  {
    return *t;
  }

  const Value_T& operator*() const
  {
    return *t;
  }

  Value_T* get()
  {
    return t.get();
  }

  const Value_T* get() const
  {
    return t.get();
  }

  operator const T&() const
  {
    return t;
  }

  operator T&()
  {
    return t;
  }

private:
  T t;
};

template <typename T, typename U>
bool operator==(const logical_const<T>& pt1, const logical_const<U>& pt2)
{
  return pt1.t == pt2.t;
}

template <typename T, typename U>
bool operator!=(const logical_const<T>& pt1, const logical_const<U>& pt2)
{
  return pt1.t != pt2.t;
}

template <typename T, typename U>
void swap(logical_const<T>& pt1, logical_const<U>& pt2)
{
  swap(pt1.t, pt2.t);
}

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
