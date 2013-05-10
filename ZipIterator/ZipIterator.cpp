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
};

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
	auto it_end = make_zip_iterator(ints.end(), words.end());

	++it_begin;
	it_begin[3];
}

