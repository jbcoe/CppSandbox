#include <iostream>
#include <sstream>

int main(int argc, char* argv[])
{
  double x = 3.14159;
  std::cout << x << std::endl;

  std::stringstream ss;
  ss << std::hexfloat << x;
  std::cout << ss.str() << std::endl;

  ss >> std::hexfloat >> x;
  std::cout << x << std::endl;
}
