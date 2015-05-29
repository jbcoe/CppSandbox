#include <iostream>
#include <type_traits>
#include <string>

template <typename T>
struct ClassWithSFINAEdFunctions
{
  template <typename T_ = T, typename std::enable_if<std::is_same<T_, int>::value,bool>::type = true>
  void IAm()
  {
    std::cout << "I hold an int " << t_ << "\n";
  }

  template <typename T_ = T, typename std::enable_if<std::is_same<T_, std::string>::value,bool>::type = false>
  void IAm()
  {
    std::cout << "I hold a string " << t_ << "\n";
  }

  T t_ = T();
};

int main(int argc, char* argv[])
{
  ClassWithSFINAEdFunctions<int> iw;
  iw.IAm();

  ClassWithSFINAEdFunctions<std::string> is;
  is.IAm();
}
