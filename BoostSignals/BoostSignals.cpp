#include <iostream>
#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>
#include <boost/signal.hpp>
#include <vector>
#include <numeric>

template <typename T>
void print_all(T first, T last)
{
  if (first == last) return;
  while (first != last) std::cout << *first++ << ' ';
  std::cout << std::endl;
}

template <typename T>
void print_sum(T first, T last)
{
  std::cout << std::accumulate(first, last, 0) << std::endl;
}

typedef std::vector<double>::const_iterator DVec_it;


void Hello(const std::string& name)
{
  std::cout << "Hello " << name << std::endl;
}


int main(int argc, char* argv[])
{
  auto HelloWorld = boost::bind(Hello, "World");

  boost::signal<void()> signal;
  signal.connect(HelloWorld);
  signal.connect(boost::bind(Hello, "Natalia"));

  signal();

  boost::signal2<void, DVec_it, DVec_it> listSignal;

  std::vector<double> myList = {1.0, 2.0, 3.0};

  listSignal.connect(&print_all<DVec_it>);

  listSignal.connect(&print_sum<DVec_it>);

  listSignal.connect(
      boost::bind(std::accumulate<DVec_it, double>, _1, _2, 0.0));

  listSignal(myList.begin(), myList.end());
}
