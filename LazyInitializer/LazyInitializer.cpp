#include <iostream>
#include <thread>
#include <Common/ScopedFunction.h>

template <typename Value_t, typename Initializer_t=std::function<Value_t (void)>>
class LazyInitialized
{
	public:
		LazyInitialized(Initializer_t i) : m_i(i), m_v() {}
		
		LazyInitialized(LazyInitialized<Value_t, Initializer_t>&& that) : m_i(std::move(that.m_i)), m_v(std::move(that.m_v)) {}
		
		LazyInitialized<Value_t, Initializer_t>& operator = (LazyInitialized<Value_t, Initializer_t>&& that) 
		{
			m_i = std::move(that.m_i);
			m_v = std::move(that.m_v);
			m_initFlag = std::move(that.m_initFlag);
		}	
		
		operator Value_t() 
		{ 
			std::call_once(m_initFlag, [this]{ m_v = m_i();});
			return m_v;
		}

	private:
		Initializer_t m_i;
		Value_t m_v;
		std::once_flag m_initFlag;
};

template<typename Value_t, typename Initializer_t>
	LazyInitialized<Value_t,Initializer_t> 
make_lazy_initialized(Initializer_t i) 
{
	return LazyInitialized<Value_t,Initializer_t>(i);
}

struct ClassWithLazyMembers
{
	LazyInitialized<double> m_baseLazyValue;
	LazyInitialized<double> m_lazyValue;

	ClassWithLazyMembers() : 
		m_baseLazyValue{[]()->double{auto f = make_scoped_function([]{std::cout << "Built base member variable" << std::endl;}); return 3.0;}},
		m_lazyValue{[this]()->double{auto f = make_scoped_function([]{std::cout << "Built derived member variable" << std::endl;}); return 2.0 * m_baseLazyValue;}} {}
};

int main(int argc, char* argv[])
{
	auto x = make_lazy_initialized<double>([]()->double{std::cout << "Built variable" << std::endl; return 2.0;});
	std::cout << x << std::endl;
	std::cout << x << std::endl;
	std::cout << std::endl;

	ClassWithLazyMembers c;
	std::cout << c.m_lazyValue << std::endl;
	std::cout << c.m_lazyValue << std::endl;
}

