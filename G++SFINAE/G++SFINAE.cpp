#include <iostream>
#include <type_traits>
#include <string>

template <typename T>
struct ClassWithSFINAEdFunctions
{
  typename std::enable_if<std::is_same<T,int>::value>::type
  IAmAnIntWrapper()
  {
    std::cout << "I hold an int" << t_ << "\n";
  }
  
  typename std::enable_if<std::is_same<T,std::string>::value>::type
  IAmAStringWrapper()
  {
    std::cout << "I hold an int" << t_ << "\n";
  }

  T t_;
};

int main(int argc, char* argv[])
{
  ClassWithSFINAEdFunctions<int> iw;
  iw.IAmAnIntWrapper();
}
