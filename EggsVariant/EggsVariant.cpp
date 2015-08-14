#include <iostream>
#include <eggs/variant.hpp>
#include <vector>
#include <map>
#include <string>

using namespace std::literals::string_literals;

class value;
using array = std::vector<value>;
using dict = std::map<std::string, value>;

// Immutabale value-type representing a JSON fragment  
class value : eggs::variant<std::string, double, int, bool, array, dict>
{
  using variant_t = eggs::variant<std::string, double, int, bool, array, dict>;
  using variant_t::variant;

public:

  template <typename T, typename U=void>
  U apply(T&& t) const 
  {
    return eggs::variants::apply<U>(std::forward<T>(t), static_cast<const variant_t&>(*this)); 
  }
};

auto indent = ""s;

struct Indenter
{
  Indenter() { indent.push_back(' ');}
  ~Indenter() { indent.pop_back();}
};

auto& os = std::cout;

struct JSONPrinter
{
  void operator()(const std::string& s)
  {
    os << s;
  }
  
  void operator()(double d)
  {
    os << d;
  }
  
  void operator()(int i)
  {
    os << i;
  }
  
  void operator()(bool b)
  {
    os << b;

  }

  void operator()(const array& a)
  {
    os << "[\n";
    {        
      Indenter in;
      for (const auto& v : a)
      {
        os << indent;
        v.apply(*this);
        os << ",\n";
      }
    }
    os << indent << "]";
  }
  
  void operator()(const dict& d)
  {
    os << "{\n";
    {
      Indenter in;
      for (const auto& kv : d)
      {
        os << indent << kv.first << ':';
        kv.second.apply(*this);
        os << ",\n";
      }
    }
    os << indent << "}";
  }
};

int main(int argc, char* argv[])
{
  value v("testing some JSON-handling"s);
  value av(std::vector<value>(3,v));
  std::map<std::string,value> m;
  m["Data"] = av;
  m["Number"] = 1;
  value mv=m;
  value mav =(std::vector<value>(2,mv)); 
  mav.apply(JSONPrinter());
  std::cout << '\n';
}

