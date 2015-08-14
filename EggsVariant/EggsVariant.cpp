#include <iostream>
#include <eggs/variant.hpp>
#include <vector>
#include <map>
#include <string>

using namespace std::literals::string_literals;

class value;
using array = std::vector<value>;
using dict = std::map<std::string, value>;

class value : eggs::variant<std::string, double, int, bool, array, dict>
{
  using variant_t = eggs::variant<std::string, double, int, bool, array, dict>;
  using variant_t::variant;

public:

  template <typename T, typename U=void>
  U apply(T&& t) 
  {
    return eggs::variants::apply<U>(std::forward<T>(t), static_cast<variant_t&>(*this)); 
  }
};

auto& os = std::cout;

struct JSONPrinter
{
  void operator()(const std::string& s) { os << s; }
  void operator()(double d) { os << d; }
  void operator()(int i) { os << i; }
  void operator()(bool b) { os << b; }
  template <typename T>
  void operator()(const T&) {}
};

int main(int argc, char* argv[])
{
  value v("testing some JSON-handling"s);

  v.apply(JSONPrinter());
}

