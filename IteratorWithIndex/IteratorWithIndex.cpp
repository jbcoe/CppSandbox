#include <iostream>
#include <vector>

template <typename Iterator_T>
class IteratorWithIndex
{
private:
  Iterator_T m_it;
  size_t m_i;

public:
  IteratorWithIndex(Iterator_T it, size_t i = 0) : m_it(std::move(it)), m_i(i) {}

  IteratorWithIndex& operator++()
  {
    ++m_i;
    ++m_it;
    return *this;
  }

  bool operator==(const IteratorWithIndex& i) const { return m_it == i.m_it; }
  bool operator==(const Iterator_T& i) const { return m_it == i; }
  
	bool operator!=(const IteratorWithIndex& i) const { return m_it != i.m_it; }
	bool operator!=(const Iterator_T& i) const { return m_it != i; }

  auto index() const { return m_i; }

  auto operator*() const { return std::make_pair(m_i,std::cref(*m_it)); }
  auto operator*() { return std::make_pair(m_i,std::ref(*m_it)); }
};

template<typename Iterator_T>
class IndexedRange
{
	IteratorWithIndex<Iterator_T> begin_;
	IteratorWithIndex<Iterator_T> end_;

public:
	IndexedRange(Iterator_T begin, Iterator_T end) : begin_(std::move(begin)), end_(std::move(end)) {} 

	auto begin() { return begin_; }
	auto end() { return end_; }
};

template<typename T>
auto make_indexed_range(T& t)
{
	typedef decltype(t.begin()) Iterator_T; 
	return IndexedRange<Iterator_T>(t.begin(),t.end());
}

int main(int argc, char* argv[]) 
{
	std::vector<std::string> v = {"I","II","III","IV","V"};

  auto iv = make_indexed_range(v); 

	for(auto x: iv)
	{
		std::cout << x.first << " : " << x.second << "\n";
	}
}
