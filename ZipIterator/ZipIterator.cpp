#include <iostream>
#include <Common/TupleUtils.h>
#include <vector>

namespace detail
{
  template <int N, bool B = false>
  struct DerefencedTuplesAreEqual
  {
    template <typename... Ts>
    bool operator()(const std::tuple<Ts...>& t1,
                    const std::tuple<Ts...>& t2) const
    {
      if (*std::get<N>(t1) != *std::get<N>(t2))
      {
        return false;
      }

      return DerefencedTuplesAreEqual<N + 1, N + 1 == sizeof...(Ts)>()(t1, t2);
    }
  };

  template <int N>
  struct DerefencedTuplesAreEqual<N, true>
  {
    template <typename... Ts>
    bool operator()(const std::tuple<Ts...>& t1,
                    const std::tuple<Ts...>& t2) const
    {
      return true;
    }
  };
} // namespace detail

template <typename... Iterators>
class ZipIterator
{
public:
  ZipIterator(Iterators... iterators) : m_iterators(iterators...) {}

  ZipIterator<Iterators...>& operator++()
  {
    TupleUtils::ApplyFunction(Increment(), m_iterators);
    return *this;
  }

  ZipIterator<Iterators...> operator[](size_t i) const
  {
    auto advanced_it = *this;
    while (i != 0)
    {
      ++advanced_it;
      --i;
    }
    return advanced_it;
  }

  bool operator==(const ZipIterator<Iterators...>& z) const
  {
    return detail::DerefencedTuplesAreEqual<0>()(this->m_iterators,
                                                 z.m_iterators);
  }

  bool operator!=(const ZipIterator<Iterators...>& z) const
  {
    return !(*this == z);
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
  auto Get() -> decltype(*std::get<N>(m_iterators))
  {
    return *std::get<N>(m_iterators);
  }
};

template <typename... Iterators>
ZipIterator<Iterators...> make_zip_iterator(Iterators... iterators)
{
  return ZipIterator<Iterators...>(iterators...);
}

int main(int argc, char* argv[])
{
  std::vector<int> ints = {1, 2, 3, 4, 5};
  std::vector<std::string> words = {"one", "two", "three", "four", "five"};
  std::vector<std::string> romans = {"I", "II", "III", "IV", "V"};

  auto it_begin =
      make_zip_iterator(ints.begin(), words.begin(), romans.begin());
  auto it_end = make_zip_iterator(ints.end(), words.end(), romans.end());

  std::cout << it_begin[3].Get<0>() << std::endl;

  for (; it_begin != it_end; ++it_begin)
  {
    std::cout << it_begin.Get<0>() << " " << it_begin.Get<1>() << " "
              << it_begin.Get<2>() << std::endl;
  }
}
