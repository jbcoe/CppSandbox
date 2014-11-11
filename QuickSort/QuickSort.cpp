#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <iostream>
#include <iterator>

using std::swap;

template<typename Iterator, typename Predicate>
std::pair<Iterator,Iterator> partition_(const Iterator begin, const Iterator end, Predicate p)
{
  auto pivot_value = *(begin+(end-begin)/2);

  std::cout << "Pivot: " << pivot_value << std::endl;

  Iterator write_low = begin;
  Iterator write_high = end;
  for(Iterator it=begin; it!=write_high;)
  {
    /*
    std::string sHL(20, ' ');
    sHL[2*std::distance(begin,write_low)] = 'L';
    sHL[2*std::distance(begin,write_high)-2] = 'H';
    std::cout << sHL << std::endl;
    std::ostream_iterator<int> osi(std::cout," ");
    std::copy(begin, end, osi);
    std::cout << std::endl;
  
    std::string s(20, ' ');
    s[2*std::distance(begin,it)] = '^';
    std::cout << s << std::endl;
    std::cout << std::endl;
    */
    if( p(*it,pivot_value) )
    {
      swap(*it,*write_low);
      ++write_low;
      ++it;
    }
    else if( p(pivot_value,*it) )
    {
      swap(*it,*--write_high);
    }
    else
    {
      ++it;
    }
  }

  //std::string s; std::getline(std::cin,s);
  return {write_low,write_high};
}

template<typename Iterator, typename Predicate>
void quick_sort(Iterator begin, Iterator end, const Predicate& p)
{
  if(end-1>begin) 
  {
    auto it_low_high = partition_(begin,end,p);
  
    quick_sort(begin, it_low_high.first, p);
    quick_sort(it_low_high.second, end, p);
  }
}

int main(int argc, char* argv[])
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> u(0,9);

  const size_t SIZE = 10;

  for(int i=0; i<100; ++i)
  {
  std::vector<int> ints(SIZE);
  std::generate_n(ints.begin(), SIZE, [&]{return u(mt);});

  //std::vector<int> ints = {1,3};

  std::ostream_iterator<int> osi(std::cout," ");
  std::copy(ints.begin(), ints.end(), osi);
  std::cout << std::endl;

  partition_(ints.begin(), ints.end(), std::less<>());
  
  std::copy(ints.begin(), ints.end(), osi);
  std::cout << std::endl;
  std::cout << std::endl;
  }
}


