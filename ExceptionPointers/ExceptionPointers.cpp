#include <iostream>
#include <iostream>
#include <vector>

class Exception_A : public std::exception
{
};

class Exception_B : public std::exception
{
};

void Thrower(int i)
{
  if ((i % 2) == 0)
  {
    throw Exception_A();
  }
  else
  {
    throw Exception_B();
  }
}

int main(int argc, char* argv[])
{
  int count = 4;
  std::vector<std::exception_ptr> exceptionPointers(count);
  for (int i = 0; i < count; ++i)
  {
    try
    {
      Thrower(i);
    }
    catch (...)
    {
      exceptionPointers[i] = std::current_exception();
    }
  }

  for (int i = 0; i < exceptionPointers.size(); ++i)
  {
    try
    {
      if (!(exceptionPointers[i] == 0))
      {
        std::rethrow_exception(exceptionPointers[i]);
      }
    }
    catch (const Exception_A& ea)
    {
      std::cerr << "Caught an exception of type A" << std::endl;
    }
    catch (const Exception_B& eb)
    {
      std::cerr << "Caught an exception of type B" << std::endl;
    }
  }
}
