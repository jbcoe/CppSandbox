#include <iostream>
#include <memory>

template <typename T>
class LogicalConst
{
public:

  typedef std::remove_reference_t<decltype(*std::declval<T>())> Value_T;

  LogicalConst(T&& t_) : t(std::move(t_)) {}
  LogicalConst(const T& t_) : t(t_) {}

  Value_T* operator->()
	{
  	return &*t;
	}

  const Value_T* operator->() const
	{
  	return &*t;
	}

private:
	T t;
};

struct A
{
	void foo() const { std::cout << "foo (const)" << std::endl; }
	void bar() { std::cout << "bar (non-const)" << std::endl; }
};

struct B
{
	B() : m_ptrA(std::make_unique<A>()) {} 

  void operator()()
	{
		m_ptrA->bar();
	}

  void operator()() const
	{
		m_ptrA->bar();
	}
	
	//LogicalConst<std::unique_ptr<A>> m_ptrA;
	std::unique_ptr<A> m_ptrA;
};

int main(int argc, char* argv[])
{
	const B b;
	b();
}

