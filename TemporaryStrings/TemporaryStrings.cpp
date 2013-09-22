#include <iostream>
#include <Common/Timer.h>

std::string GetBigString()
{
  return "A string with sufficient size to defeat small string optimisation";
}

const std::string GetConstBigString()
{
  return "A string with sufficient size to defeat small string optimisation";
}

int main(int argc, char* argv[])
{
  size_t repeat = 1000000;

  {
    auto t = make_timer("Const Big String");
    for (int i = 0; i < repeat; ++i)
    {
      std::string s = GetConstBigString();
      if (s.empty())
        return -1;
    }
  }
  {
    auto t = make_timer("Const Big String c_str");
    for (int i = 0; i < repeat; ++i)
    {
      std::string s = GetConstBigString().c_str();
      if (s.empty())
        return -1;
    }
  }
  
	{
    auto t = make_timer("Big String");
    for (int i = 0; i < repeat; ++i)
    {
      std::string s = GetBigString();
      if (s.empty())
        return -1;
    }
  }
  {
    auto t = make_timer("Big String c_str");
    for (int i = 0; i < repeat; ++i)
    {
      std::string s = GetBigString().c_str();
      if (s.empty())
        return -1;
    }
  }
  return 0;
}

