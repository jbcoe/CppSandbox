#include <iostream>

template <int argumentCount>
int _countTemplateArguments()
{
  return argumentCount;
}

template <int argumentCount, typename T, typename... Values>
int _countTemplateArguments()
{
  return _countTemplateArguments<argumentCount + 1, Values...>();
}

template <typename... Values>
int countTemplateArguments(Values...)
{
  return _countTemplateArguments<0, Values...>();
}

int main(int argc, char* argv[])
{
  std::cout << "Template function was provided with "
            << countTemplateArguments(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
            << " arguments." << std::endl;
}
