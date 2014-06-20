#include <iostream>
#include <memory>

template <typename T>
class propagate_const
{
  typedef decltype(*std::declval<T>()) reference_type;

public:

	using value_type = typename std::enable_if<
      std::is_lvalue_reference<reference_type>::value,
      typename std::remove_reference<reference_type>::type>::type;

  propagate_const() = default;

  template <typename U, typename V = typename std::enable_if<std::is_convertible<U,T>::value>::type>
  propagate_const(U&& u) : t{std::forward<U>(u)} {}
	
  template <typename U>
  propagate_const(const propagate_const<U>& pu) : t(pu.t) {}

  template <typename U>
  propagate_const(propagate_const<U>&& pu) : t(std::move(pu.t)) {}

  template <typename U, typename V = typename std::enable_if<std::is_convertible<U,T>::value>::type>
  propagate_const<T>& operator=(U&& u)
  {
    t = std::forward<U>(u);
    return *this;
  }

	template <typename U>
  propagate_const<T>& operator=(const propagate_const<U>& pt)
  {
    t = pt.t;
    return *this;
  }

  template <typename U>
  propagate_const<T>& operator=(propagate_const<U>&& pt)
  {
    t = std::move(pt.t);
    return *this;
  }

  value_type* operator->() { return underlying_pointer(t); }

  const value_type* operator->() const { return underlying_pointer(t); }

  value_type* get() { return underlying_pointer(t); }

  const value_type* get() const { return underlying_pointer(t); }

  operator value_type*() { return underlying_pointer(t); }

  operator const value_type*() const { return underlying_pointer(t); }

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

  propagate_const<std::unique_ptr<A>> m_ptrA;
};

int main(int argc, char* argv[])
{
  const B b;
  b();

	A a;
	propagate_const<A*> pcA(&a);
	static_assert(std::is_trivially_destructible<decltype(pcA)>::value, "Not trivially destructible");
	static_assert(std::is_trivially_move_constructible<decltype(pcA)>::value, "Not trivially move constructible");

	propagate_const<A*> pcA2(pcA);

	auto shptrA = std::make_shared<A>();
	propagate_const<std::shared_ptr<A>> pcsptrA(shptrA);
	propagate_const<std::shared_ptr<A>> pcsptrA2(pcsptrA);
	propagate_const<std::shared_ptr<A>> pcsptrA3(std::move(pcsptrA));
	
	propagate_const<std::unique_ptr<A>> pcuptrA(std::make_unique<A>());
	propagate_const<std::unique_ptr<A>> pcuptrA2(std::move(pcuptrA));
}
