#include <iostream>

template <typename T, T defaultValue>
struct TypeWithDefaultValue
{
  TypeWithDefaultValue() : m_value(defaultValue)
  {
  }

  template <typename U>
  TypeWithDefaultValue(U&& u)
      : m_value(std::forward<U>(u))
  {
  }

  template <typename U>
  TypeWithDefaultValue<T, defaultValue>& operator=(U&& u)
  {
    m_value = std::forward<U>(u);
  }

  operator T&()
  {
    return m_value;
  }

  operator const T&() const
  {
    return m_value;
  }

  T m_value;
};

int main(int argc, char* argv[])
{
  TypeWithDefaultValue<int, 3> myInt3;
  std::cout << myInt3 << std::endl;

  myInt3 *= 2;

  std::cout << myInt3 << std::endl;
}
