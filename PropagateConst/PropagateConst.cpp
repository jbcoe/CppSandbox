#include <iostream>
#include <memory>

template <typename T>
class propagate_const
{
  typedef decltype(*std::declval<T>()) reference_type;

public:
 
	static_assert(std::is_lvalue_reference<reference_type>::value,"");

	using value_type = std::remove_reference_t<reference_type>;

  propagate_const() = default;

  template <typename U, typename Dummy = typename std::enable_if<std::is_convertible<U,T>::value>::type>
  propagate_const(U&& u) : t{std::forward<U>(u)} {}
	
  template <typename U, typename Dummy = typename std::enable_if<std::is_convertible<U,T>::value>::type>
  propagate_const(const propagate_const<U>& pu) : t(pu.t) {}

  template <typename U, typename Dummy = typename std::enable_if<std::is_convertible<U,T>::value>::type>
  propagate_const(propagate_const<U>&& pu) : t(std::move(pu.t)) {}

  template <typename U, typename Dummy = typename std::enable_if<std::is_convertible<U,T>::value>::type>
  propagate_const<T>& operator=(U&& u)
  {
    t = std::forward<U>(u);
    return *this;
  }

  template <typename U, typename Dummy = typename std::enable_if<std::is_convertible<U,T>::value>::type>
  propagate_const<T>& operator=(const propagate_const<U>& pt)
  {
    t = pt.t;
    return *this;
  }

  template <typename U, typename Dummy = typename std::enable_if<std::is_convertible<U,T>::value>::type>
  propagate_const<T>& operator=(propagate_const<U>&& pt)
  {
    t = std::move(pt.t);
    return *this;
  }

  value_type* operator->() { return underlying_pointer(t); }

  const value_type* operator->() const { return underlying_pointer(t); }

  value_type* get() { return underlying_pointer(t); }

  const value_type* get() const { return underlying_pointer(t); }

  template <typename Duplicate=T, typename Dummy = typename std::enable_if<std::is_pointer<Duplicate>::value>::type>
  operator value_type*() 
	{ 
		return underlying_pointer(t); 
	}

  template <typename Duplicate=T, typename Dummy = typename std::enable_if<std::is_pointer<Duplicate>::value>::type>
  operator const value_type*() const 
	{ 
		return underlying_pointer(t); 
	}

  value_type& operator*() { return *t; }

  const value_type& operator*() const { return *t; }

  explicit operator bool() const noexcept(noexcept(bool(std::declval<T>()))) { return static_cast<bool>(t); }

	template<typename U, typename V>
	friend bool operator == (const propagate_const<U>& pt, const propagate_const<V>& pu);

	template<typename U, typename V>
	friend bool operator != (const propagate_const<U>& pt, const propagate_const<V>& pu);
	
	template<typename U, typename V>
	friend bool operator < (const propagate_const<U>& pt, const propagate_const<V>& pu);
	
	template<typename U, typename V>
	friend bool operator > (const propagate_const<U>& pt, const propagate_const<V>& pu);
	
	template<typename U, typename V>
	friend bool operator <= (const propagate_const<U>& pt, const propagate_const<V>& pu);
	
	template<typename U, typename V>
	friend bool operator >= (const propagate_const<U>& pt, const propagate_const<V>& pu);

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

template<typename T, typename U>
bool operator == (const propagate_const<T>& pt, const propagate_const<U>& pu)
{
	return pt.t == pu.t;
}

template<typename T, typename U>
bool operator != (const propagate_const<T>& pt, const propagate_const<U>& pu)
{
	return pt.t != pu.t;
}

template<typename T, typename U>
bool operator < (const propagate_const<T>& pt, const propagate_const<U>& pu)
{
	return pt.t < pu.t;
}

template<typename T, typename U>
bool operator > (const propagate_const<T>& pt, const propagate_const<U>& pu)
{
	return pt.t > pu.t;
}

template<typename T, typename U>
bool operator <= (const propagate_const<T>& pt, const propagate_const<U>& pu)
{
	return pt.t <= pu.t;
}

template<typename T, typename U>
bool operator >= (const propagate_const<T>& pt, const propagate_const<U>& pu)
{
	return pt.t >= pu.t;
}

struct A
{
  void bar() const { std::cout << "bar (const)" << std::endl; }
  void bar() { std::cout << "bar (non-const)" << std::endl; }
};

struct B
{
  B() : m_ptrA(std::make_unique<A>()) {}

  void operator()() 
	{ 
	  std::cout << "foo (non-const)" << std::endl;
		m_ptrA->bar(); 
	}

  void operator()() const 
	{ 
	  std::cout << "foo (const)" << std::endl;
		m_ptrA->bar(); 
	}

  propagate_const<std::unique_ptr<A>> m_ptrA;
};

int main(int argc, char* argv[])
{
  B b;
  b();
  
	const B cb;
  cb();

	A a;
	propagate_const<A*> pcA(&a);
	propagate_const<A*> pcA2(pcA);
	propagate_const<A*> pcA3(std::move(pcA2));

	static_assert(std::is_trivially_destructible<decltype(pcA)>::value, "Not trivially destructible");
	static_assert(std::is_trivially_move_constructible<decltype(pcA)>::value, "Not trivially move constructible");

	auto shptrA = std::make_shared<A>();
	propagate_const<std::shared_ptr<A>> pcsptrA(shptrA);
	propagate_const<std::shared_ptr<A>> pcsptrA2(pcsptrA);
	propagate_const<std::shared_ptr<A>> pcsptrA3(std::move(pcsptrA));
	
	propagate_const<std::unique_ptr<A>> pcuptrA(std::make_unique<A>());
	propagate_const<std::unique_ptr<A>> pcuptrA2(std::move(pcuptrA));
}
