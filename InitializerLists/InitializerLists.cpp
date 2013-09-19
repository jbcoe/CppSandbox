#include <iostream>

class SimpleVector
{
public:
  SimpleVector(std::initializer_list<int> list)
  {
    std::cout << "Initialised with " << list.size() << " variables"
              << std::endl;
    for (const int& v : list)
    {
      std::cout << "  value: " << v << std::endl;
    }
  }
};

int main(int argc, char* argv[])
{
  SimpleVector myVector = {1, 2, 3, 4, 5, 6, 7};
}
