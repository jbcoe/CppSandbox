#include <iostream>
#include <vector>

template <typename Iterator_T>
class IteratorWithIndex
{
private:
  Iterator_T m_it;
  size_t m_i;

public:
  IteratorWithIndex(Iterator_T it, size_t i = 0) : m_it(it), m_i(i) {}

  IteratorWithIndex& operator++()
  {
    ++m_i;
    ++m_it;
    return *this;
  }

  bool operator==(const IteratorWithIndex& i) const { return m_it == i.m_it; }

  auto operator*() const { return std::make_pair(*m_it, m_i); }
};

int main(int argc, char* argv[]) {}
