#include <iostream>

struct A
{
};

int main(int argc, char* argv[])
{
  const auto a = A();
  const A aa{};
  //const A aaa;
}

