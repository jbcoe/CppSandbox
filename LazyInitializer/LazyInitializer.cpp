#include <iostream>

template <typename Value_t, typename Initializer_t=std::function<Value_t (void)>>
class LazyInitialized
{
	public:
		LazyInitialized(Initializer_t i) : m_i(i), m_v(), m_needInit(true) {}
		operator Value_t() 
		{ 
			if ( m_needInit ) m_v = m_i();
			m_needInit = false;
			return m_v;
		}

	private:
		Initializer_t m_i;
		Value_t m_v;
		bool m_needInit;
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
		m_baseLazyValue{[]()->double{return 2.0;}},
		m_lazyValue{[this]()->double{std::cout << "Built member variable" << std::endl; return 2.0 * m_baseLazyValue;}} {}
	
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

