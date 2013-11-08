#include <iostream>
#include <sstream>
#include <random>
#include <functional>
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

    if (f_(*iterator_))
    {
      return *this;
    }
    else
    {
      while (iterator_ != end_ && !f_(*iterator_))
      {
        ++iterator_;
      }
    }

    return *this;
  }

  ConditionalIterator(Iterator_t iterator, Iterator_t end, Functor_t f)
      : iterator_(iterator), end_(end), f_(f)
  {
    if (!f_(*iterator_))
    {
      while (iterator_ != end_ && !f_(*iterator_))
      {
        ++iterator_;
      }
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


template <Iterator Iterator_t, Functor Functor_t>
class TransformIterator
{
public:
  TransformIterator(Iterator_t iterator, Functor_t f)
      : iterator_(iterator), f_(f)
  {
  }

  bool operator==(TransformIterator<Iterator_t, Functor_t> that) const
  {
    return iterator_ == that.iterator_;
  }

  bool operator!=(TransformIterator<Iterator_t, Functor_t> that) const
  {
    return iterator_ != that.iterator_;
  }

  TransformIterator<Iterator_t, Functor_t>& operator++()
  {
    ++iterator_;
    return *this;
  }

private:
  Iterator_t iterator_;
  Functor_t f_;

public:
  auto operator*() const -> decltype(f_(*iterator_))
  {
    return f_(*iterator_);
  }
};

template <Iterator Iterator_t, Functor Functor_t>
std::pair<TransformIterator<Iterator_t, Functor_t>,
          TransformIterator<Iterator_t, Functor_t>>
make_transform_iterator_begin_and_end(Iterator_t begin, Iterator_t end,
                                      Functor_t f)
{
  return {TransformIterator<Iterator_t, Functor_t>(begin, f),
          TransformIterator<Iterator_t, Functor_t>(end, f)};
}


int main(int argc, char* argv[])
{
  std::uniform_int_distribution<int> distribution(0, 99);
  std::mt19937 engine;
  auto generator = std::bind(distribution, engine);

  std::vector<int> numbers;
  std::generate_n(std::back_inserter(numbers), 50, generator);

  auto even_number_iterators =
      make_conditional_begin_and_end(numbers.begin(), numbers.end(), [](int x)
  { return x % 2 == 0; });

  int count = 0;
  auto even_number_with_count_iterators = make_transform_iterator_begin_and_end(
      even_number_iterators.first, even_number_iterators.second,
      [&](int x)->std::string
  {
        std::stringstream ss;
        ss << x << ":" << count++;
        return ss.str();
      });

  std::ostream_iterator<std::string> osi(std::cout, " ");
  std::copy(even_number_with_count_iterators.first,
            even_number_with_count_iterators.second, osi);
  std::cout << std::endl;
}
