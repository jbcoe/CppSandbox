#include <iostream>
#include <exception>

void its_all_gone_quiet_over_there()
{
  std::cout << "It's all gone quiet over there\n";
}

void f() noexcept
{
  throw std::runtime_error("Boom!");
}

int main(int argc, char* argv[])
{
  std::set_terminate(its_all_gone_quiet_over_there);
  f();
}
