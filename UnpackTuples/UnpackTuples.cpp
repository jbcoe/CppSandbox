// Tuple unpacking using a tuple indexer
//
// Adapted from: http://loungecpp.wikidot.com/tips-and-tricks%3aindices
//
// tuple<A,B,C> result =
// 	tuple<A,B,C>(f(get<0>(x), f(get<1>(x), f(get<2>(x));
//
//	Is replaced with
//
// tuple<Args...> result = tuple<Args...>(f(get<Indices>(x))...);
//

#include <iostream>                          
#include <tuple>

using namespace std;


template <std::size_t... Is>
struct TupleIndices {};

template <std::size_t N, std::size_t... Is>
struct build_indices : build_indices<N-1, N-1, Is...> {};

template <std::size_t... Is>
struct build_indices<0, Is...> : TupleIndices<Is...> {};

template <typename ...Ts>
using IndicesFor = build_indices<sizeof...(Ts)>;


template <typename Functor, typename Tuple, std::size_t... Indices>
Tuple apply_f_impl(Functor f, const Tuple& t, TupleIndices<Indices...>) 
{
	return Tuple(f(std::get<Indices>(t))...);
}

template <typename Functor, typename ...Ts>
tuple<Ts...> apply_f(Functor f, const tuple<Ts...>& t) 
{
	return apply_f_impl(f, t, IndicesFor<Ts...> {});
}

struct MyFunctor
{
	template <typename T> 
		T operator()(const T & t) const { return 2*t; }
};

struct ToStream
{
	ToStream(ostream& os) : m_os(os) {}
	template<typename T>
		T operator() (T t) 
		{
			m_os << t << ",";
			return t;
		}

	ostream& m_os;
};

template<typename ...Ts>
ostream& operator << (ostream& os, tuple<Ts...>t)
{
	os << '(';
	apply_f(ToStream(os), t);
	os << "\b)";
	return os;
}

int main()
{
	tuple<int, double, char> x(5, 1.5, 'a');
	auto y = apply_f(MyFunctor(),x);

	cout << "Before: " << x << " After: " << y << endl;
}

