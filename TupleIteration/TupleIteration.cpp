#include <iostream>
#include <tuple>

template <int N, typename Functor_t, typename... Ts>
struct ApplyImpl
{
	void operator() (Functor_t f, std::tuple<Ts...> t)
	{
		f(std::get<sizeof...(Ts)-N>(t));
		ApplyImpl<N-1, Functor_t, Ts...>()(f,t);
	}
};

template <typename Functor_t, typename... Ts>
struct ApplyImpl<0, Functor_t, Ts...>
{
	void operator() (Functor_t f, std::tuple<Ts...> t)
	{
		return;
	}
};

template <typename Functor_t, typename... Ts>
void Apply(Functor_t f, std::tuple<Ts...> t)
{
	ApplyImpl<(int)(sizeof...(Ts)), Functor_t, Ts...>()(f,t); 
}

struct Greeter
{
		void operator()(double v)
		{
			std::cout << "Hello double" << std::endl;
		}
		void operator()(float v)
		{
			std::cout << "Hello float" << std::endl;
		}
		void operator()(int v)
		{
			std::cout << "Hello int" << std::endl;
		}
		void operator()(char v)
		{
			std::cout << "Hello char" << std::endl;
		}
		template <typename T>
		void operator()(const T& t)
		{
			std::cout << "Hello unhandled type" << std::endl;
		}
};

int main(int argc, char* argv[])
{
	auto myTuple = std::make_tuple(1,2.0f,3.0,'a', Greeter());
	Apply(Greeter(), myTuple);
}

