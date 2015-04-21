#include <iostream>
#include <boost/variant.hpp>

int main(int argc, char* argv[])
{
  boost::variant<int,double,char> v;

  struct printer : boost::static_visitor<>
  {
    void operator()(const int i) const { std::cout << i << "\n"; }
    void operator()(const double d) const { std::cout << d << "\n"; }
    void operator()(const char c) const { std::cout << c << "\n"; }
  };

  v = 7;
  boost::apply_visitor(printer(), v);
  
  v = 3.14159;
  boost::apply_visitor(printer(), v);
  
  v = 'p';
  boost::apply_visitor(printer(), v);
}

