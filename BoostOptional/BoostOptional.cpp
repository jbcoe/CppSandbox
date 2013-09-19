#include <iostream>
#include <boost/optional.hpp>

using namespace boost;

int main(int argc, char* argv[])
{
  optional<int> noInt;
  optional<int> numberFive(5);

  if (noInt)
  {
    std::cout << *noInt << std::endl;
  }

  if (numberFive)
  {
    std::cout << *numberFive << std::endl;
  }
}
