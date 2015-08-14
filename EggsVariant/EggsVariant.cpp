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
};

int main(int argc, char* argv[])
{
  value v("testing some JSON-handling"s);
}

