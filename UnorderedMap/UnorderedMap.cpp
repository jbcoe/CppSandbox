#include <iostream>
#include <unordered_map>

class MyClass
{
  friend struct std::hash<MyClass>;
  friend std::ostream& operator<<(std::ostream& os, const MyClass& m);

public:

  MyClass(double value) : m_value(value)
  {
  }

  MyClass(const MyClass& m) : m_value(m.m_value)
  {
  }

  MyClass& operator=(const MyClass& m)
  {
    m_value = m.m_value;
    return *this;
  }

  bool operator==(const MyClass& m) const
  {
    return m.m_value == m_value;
  }

private:
  double m_value;
};

std::ostream& operator<<(std::ostream& os, const MyClass& m)
{
  os << m.m_value;
  return os;
}

namespace std
{
  template <>
  struct hash<MyClass>
  {
    const size_t operator()(const MyClass& m) const
    {
      return hash<decltype(m.m_value)>().operator()(m.m_value);
    }
  };
}

int main(int argc, char* argv[])
{
  std::unordered_map<MyClass, std::string> map_myClass_string;
  map_myClass_string[55.4] = "first inserted value";
  map_myClass_string[35.] = "second inserted value";

  for (const auto& pair : map_myClass_string)
  {
    std::cout << pair.first << " " << pair.second << std::endl;
  }
}
