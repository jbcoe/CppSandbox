#include <iostream>
#include <Common/TupleUtils.h>
#include <vector>

template <typename ...Iterators>
class ZipIterator
{
public:
  ZipIterator(Iterators... iterators) : m_iterators(iterators...) {}

	ZipIterator<Iterators...>& operator ++ ()
	{
		TupleUtils::ApplyFunction(Increment(),m_iterators);
		return *this;
	}

	ZipIterator<Iterators...> operator[](size_t i) const
	{
		auto advanced_it = *this;
		while ( i != 0 )
		{
			++advanced_it;
			--i;
		}
    return advanced_it;
	}


private:
  struct Increment
	{
		template <typename T>
		T& operator()(T& t)
		{
			return ++t;
		}
	};

	std::tuple<Iterators...> m_iterators;
	
public:

	template <int N>
  static auto Get(ZipIterator<Iterators...>& z) -> decltype(*std::get<N>(z.m_iterators))
	{
		return *std::get<N>(z.m_iterators);
	}

};

#define ZipIterator_Get(N,x) decltype(x)::Get<N>(x)

template <typename ...Iterators>
ZipIterator<Iterators...> make_zip_iterator(Iterators... iterators)
{
	return ZipIterator<Iterators...>(iterators...);
}

int main(int argc, char* argv[])
{
	std::vector<int> ints = {1,2,3,4,5};
	std::vector<std::string> words = {"one","two","three","four","five"};

	auto it_begin = make_zip_iterator(ints.begin(), words.begin());

	for (size_t i=0; i<ints.size(); ++i, ++it_begin)
	{
		std::cout << ZipIterator_Get(0,it_begin) << " " << ZipIterator_Get(1,it_begin) << std::endl;
	} 
}

