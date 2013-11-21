#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

#define Iterator typename
#define Functor typename

template <Iterator Iterator_t, Functor Functor_t>
class ConditionalIterator
{
public:
  bool operator==(ConditionalIterator<Iterator_t, Functor_t> that) const
  {
    return iterator_ == that.iterator_;
  }

  bool operator!=(ConditionalIterator<Iterator_t, Functor_t> that) const
  {
    return iterator_ != that.iterator_;
  }

  ConditionalIterator<Iterator_t, Functor_t>& operator++()
  {
    if (iterator_ == end_)
      return *this;

    ++iterator_;

    while (iterator_ != end_ && !f_(*(iterator_)))
    {
      ++iterator_;
    }

    return *this;
  }

  ConditionalIterator(Iterator_t iterator, Iterator_t end, Functor_t f)
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
  Functor_t f_;

public:
  auto operator*() const -> decltype(*iterator_)
  {
    return *iterator_;
  }
};


template <Iterator Iterator_t, Functor Functor_t>
std::pair<ConditionalIterator<Iterator_t, Functor_t>,
          ConditionalIterator<Iterator_t, Functor_t>>
make_conditional_begin_and_end(Iterator_t begin, Iterator_t end,
                               Functor_t condition)
{
  return {ConditionalIterator<Iterator_t, Functor_t>(begin, end, condition),
          ConditionalIterator<Iterator_t, Functor_t>(end, end, condition)};
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
