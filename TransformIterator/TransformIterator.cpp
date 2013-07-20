#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

#define Iterator typename
#define Functor typename

template <Iterator Iterator_t, Functor Functor_t>
class TransformIterator
{
	public:

    TransformIterator(Iterator_t iterator, Functor_t f) : iterator_(iterator), f_(f) {}

		bool operator == (TransformIterator<Iterator_t,Functor_t> that) const
		{
			return iterator_ == that.iterator_;
		}              

		bool operator != (TransformIterator<Iterator_t,Functor_t> that) const
		{
			return iterator_ != that.iterator_;
		}              

		TransformIterator<Iterator_t,Functor_t>& operator ++ ()
		{
			++iterator_;
			return *this;
		}

	private:
    Functor_t f_;
		Iterator_t iterator_;

	public:

		auto operator* () const -> decltype(f_(*iterator_))
		{
			return f_(*iterator_);
		}

};

template <Iterator Iterator_t, Functor Functor_t>
std::pair<TransformIterator<Iterator_t,Functor_t>, TransformIterator<Iterator_t,Functor_t>> 
make_transform_iterator_begin_and_end(Iterator_t begin, Iterator_t end, Functor_t f)
{
	return { TransformIterator<Iterator_t, Functor_t>(begin,f), TransformIterator<Iterator_t, Functor_t>(end,f) };
}

int main(int argc, char* argv[])
{
	std::vector<int> numbers = {1,2,3,4,5};
	auto roman_begin_and_end = make_transform_iterator_begin_and_end(numbers.begin(), numbers.end(), [](int x) -> std::string
			{
			switch (x)
			{
      	case 1:
					return "I";
				case 2:
					return "II";
				case 3:
					return "III";
				case 4:
					return "IV";
				case 5:
					return "V";
				default:
					return "?";
			}              
			});

	std::ostream_iterator<std::string> osi(std::cout, " ");
	std::copy(roman_begin_and_end.first, roman_begin_and_end.second, osi);
	std::cout << std::endl;
}

