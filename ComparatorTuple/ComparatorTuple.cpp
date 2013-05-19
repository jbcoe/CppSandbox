#include <iostream>
#include <set>
#include <tuple>
 
template <int N>
struct CompareNthWithN
{
	template<typename ...Ts, typename ...Cs>
	static bool IsLessThan(const std::tuple<Ts...>& t1, const std::tuple<Ts...>& t2, const std::tuple<Cs...>& c)
	{
  	auto cn = std::get<N-1>(c);

		if ( cn(std::get<N-1>(t1), std::get<N-1>(t2)) )
			return true;
		if ( cn(std::get<N-1>(t2), std::get<N-1>(t1)) )
			return false;

		return CompareNthWithN<N-1>::IsLessThan(t1,t2,c);
	}
};

template <>
struct CompareNthWithN<0>
{
	template<typename ...Ts, typename ...Cs>
	static bool IsLessThan(const std::tuple<Ts...>& t1, const std::tuple<Ts...>& t2, const std::tuple<Cs...>& c)
	{
		return false;
	}
};

template <typename ...Comparator_ts>
struct ComparatorTuple
{
	template <typename ...Ts>
	bool operator ()(const std::tuple<Ts...>& t1, const std::tuple<Ts...>& t2)
	{                                                                                           
		auto comparator = std::tuple<Comparator_ts...>{};
    return CompareNthWithN<sizeof...(Ts)>::IsLessThan(t1,t2,comparator);
	}
};

struct Badger
{
	std::string name;
};

struct BadgerLessThan
{
		bool operator ()(const Badger& t1, const Badger& t2)
		{
			return t1.name < t2.name;
		}
};

int main(int argc, char* argv[])
{
	auto b1 = std::make_tuple(1,3.0,Badger{});
	auto b2 = std::make_tuple(1,2.0,Badger{});

	std::set<decltype(b1), ComparatorTuple<std::less<int>,std::less<double>,BadgerLessThan>> bs;

	bs.insert(b1);
	bs.insert(b2);
}

