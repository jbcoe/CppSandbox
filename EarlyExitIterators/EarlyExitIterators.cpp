#include <iostream>
#include <vector>

template <typename Iterator_T, typename Condition_T>
class EarlyExitIterator
{
private:
  Iterator_T m_it;
  const Iterator_T m_end;
  Condition_T m_c;

public:
  EarlyExitIterator(Iterator_T it, Iterator_T end, Condition_T c)
      : m_it(std::move(it)), m_end(std::move(end)), m_c(std::move(c))
  {
    if (m_it != m_end && m_c(*m_it))
    {
      m_it = m_end;
    }
  }

  EarlyExitIterator& operator++()
  {
    ++m_it;
    if (m_it != m_end && m_c(*m_it))
    {
      m_it = m_end;
    }
    return *this;
  }

  auto operator*() const
  {
    return *m_it;
  }

  bool operator==(const EarlyExitIterator& i)
  {
    return m_it == i.m_it;
  }

  bool operator!=(const EarlyExitIterator& i)
  {
    return !(*this == i);
  }
};

template <typename Iterable_T, typename Condition_T>
class EarlyExitRange
{
private:
  typedef decltype(std::declval<const Iterable_T>().begin()) BaseIterator_T;
  typedef EarlyExitIterator<BaseIterator_T, Condition_T> Iterator_T;

  Iterator_T m_begin;
  Iterator_T m_end;

public:
  EarlyExitRange(const Iterable_T& iterable, Condition_T c)
      : m_begin(Iterator_T(iterable.begin(), iterable.end(), c)),
        m_end(Iterator_T(iterable.end(), iterable.end(), c))
  {
  }

  auto begin() const
  {
    return m_begin;
  }
  auto end() const
  {
    return m_end;
  }
};

template <typename Iterable_T, typename Condition_T>
auto make_iterable_until(const Iterable_T i, Condition_T c)
{
  return EarlyExitRange<Iterable_T, Condition_T>(i, c);
}

int main(int argc, char* argv[])
{
  std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  for (auto x : make_iterable_until(v, [](auto i)
                                    {
                                      return i > 5;
                                    }))
  {
    std::cout << x << ' ';
  }
  std::cout << std::endl;
}
