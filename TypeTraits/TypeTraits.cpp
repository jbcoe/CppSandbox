#include <iostream>
#include <sstream>
#include <type_traits>

class CEmptyClass
{
public:
  static constexpr const char* name = "CEmptyClass";
};

class CDerivedClass : public CEmptyClass
{
public:
  static constexpr const char* name = "CDerivedClass";
};

template <typename T>
std::string TypeName()
{
  std::stringstream ss;
  if (std::is_const<typename std::remove_reference<
          typename std::remove_pointer<T>::type>::type>::value)
    ss << "const ";

  ss << std::remove_pointer<
            typename std::remove_reference<T>::type>::type::name;

  if (std::is_pointer<T>::value)
  {
    ss << '*';
  }
  if (std::is_reference<T>::value)
  {
    ss << '&';
  }

  return std::move(ss.str());
}

template <typename A_t, typename B_t>
void IsSame()
{
  std::cout << "Is '" << TypeName<A_t>() << "' the same type as '"
            << TypeName<B_t>() << "' : " << std::is_same<A_t, B_t>::value
            << std::endl;
}

template <typename A_t>
void IsConst()
{
  std::cout << "Is '" << TypeName<A_t>()
            << "' constant : " << std::is_const<A_t>::value << std::endl;
}

int main(int argc, char* argv[])
{
  IsSame<CEmptyClass, CDerivedClass>();
  IsSame<const CEmptyClass&, CEmptyClass>();
  IsSame<CEmptyClass, CEmptyClass>();
  IsConst<const CEmptyClass&>();
}
