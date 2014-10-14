#include <iostream>
#include <type_traits>
#include <string>

template <typename T>
struct ClassWithSFINAEdFunctions
{
  template <typename T_ = T, typename U = typename std::enable_if<
                                 std::is_same<T_, int>::value>::type>
  void IAmAnIntWrapper()
  {
    std::cout << "I hold an int " << t_ << "\n";
  }

  template <typename T_ = T, typename U = typename std::enable_if<
                                 std::is_same<T_, std::string>::value>::type>
  void IAmAStringWrapper()
  {
    std::cout << "I hold a string " << t_ << "\n";
  }

  T t_ = T();
};

int main(int argc, char* argv[])
{
  ClassWithSFINAEdFunctions<int> iw;
  iw.IAmAnIntWrapper();

  ClassWithSFINAEdFunctions<std::string> is;
  is.IAmAStringWrapper();
}
