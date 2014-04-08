#include <iostream>
#include <sstream>
#include <string>

class A
{
public:
  template <typename T>
  A& operator<<(const T& t)
  {
    m_buffer << t;
    return *this;
  }

  operator std::string() const
  {
    return m_buffer.str();
  }

private:
  std::stringstream m_buffer;
};

A make_A()
{
  return A();
}

int main(int argc, char* argv[])
{
  std::string s = make_A() << "Hello " << 34;
  std::cout << s << std::endl;
}
