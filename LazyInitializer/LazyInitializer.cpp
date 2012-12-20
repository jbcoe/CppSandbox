#include <iostream>

template <typename Value_t, typename Initialized_t>
class LazyInitialized
{
	public:
		LazyInitialized(Initialized_t i) : m_i(i), m_v(), m_needInit(true) {}
		operator Value_t() 
		{ 
			if ( m_needInit ) m_v = m_i();
			m_needInit = false;
			return m_v;
		}

	private:
		Initialized_t m_i;
		Value_t m_v;
		bool m_needInit;
};

template<typename Value_t, typename Initialized_t>
LazyInitialized<Value_t,Initialized_t> 
make_lazy_initialized(Initialized_t i) 
{
	return LazyInitialized<Value_t,Initialized_t>(i);
}

int main(int argc, char* argv[])
{
	auto x = make_lazy_initialized<double>([]()->double{std::cout << "Built" << std::endl; return 2.0;});
	std::cout << x << std::endl;
	std::cout << x << std::endl;
}

