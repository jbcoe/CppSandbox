#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

#define Iterator typename
#define FunctionObject typename

template <Iterator Iterator_t, FunctionObject FunctionObject_t>
class ConditionalIterator
{
public:
  bool operator==(ConditionalIterator<Iterator_t, FunctionObject_t> that) const
  {
    return iterator_ == that.iterator_;
  }

  bool operator!=(ConditionalIterator<Iterator_t, FunctionObject_t> that) const
  {
    return iterator_ != that.iterator_;
  }

  ConditionalIterator<Iterator_t, FunctionObject_t>& operator++()
  {
    // NB this is unsafe if called when an iterator is already equal to end
		
		++iterator_;

    while (iterator_ != end_ && !f_(*(iterator_)))
    {
      ++iterator_;
    }

    return *this;
  }

  ConditionalIterator(Iterator_t iterator, Iterator_t end, FunctionObject_t f)
      : iterator_(iterator), end_(end), f_(f)
  {
    while (iterator_ != end_ && !f_(*iterator_))
    {
      ++iterator_;
    }
  }

private:
  Iterator_t iterator_;
  Iterator_t end_;
  FunctionObject_t f_;

public:
  auto operator*() const { return *iterator_; }
};


template <Iterator Iterator_t, FunctionObject FunctionObject_t>
std::pair<ConditionalIterator<Iterator_t, FunctionObject_t>,
          ConditionalIterator<Iterator_t, FunctionObject_t>>
make_conditional_begin_and_end(Iterator_t begin, Iterator_t end,
                               FunctionObject_t condition)
{
  return {ConditionalIterator<Iterator_t, FunctionObject_t>(begin, end, condition),
          ConditionalIterator<Iterator_t, FunctionObject_t>(end, end, condition)};
}

int main(int argc, char* argv[])
{
  std::vector<int> numbers = {1, 2, 3, 5, 6, 7, 5, 4, 3, 2, 3,
                              4, 5, 7, 9, 2, 1, 3, 4, 5, 7, 3};

  auto isThree = [](int x)
  { return x == 3; };

  auto conditional_begin_end =
      make_conditional_begin_and_end(numbers.begin(), numbers.end(), isThree);
  std::ostream_iterator<int> osi(std::cout, " ");

  std::copy(numbers.begin(), numbers.end(), osi);
  std::cout << "\n3 appears in the list "
            << std::count_if(numbers.begin(), numbers.end(), isThree)
            << " times" << std::endl;
  std::copy(conditional_begin_end.first, conditional_begin_end.second, osi);
  std::cout << std::endl;
}
