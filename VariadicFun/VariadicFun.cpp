#include <iostream>

template <typename F, typename... Ts>
void ApplyToEach(F f, Ts... ts)
{
  for (auto x : {ts...})
  {
    f(x);
  }
}

int main(int argc, char* argv[])
{
  ApplyToEach([](auto x)
              {
                std::cout << x << " ";
              },
              1, 2, 3, 4, 5);
  std::cout << std::endl;
}
