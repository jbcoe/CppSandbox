#include <iostream>
#include <vector>

struct S {
  S(const S&)
  {
    std::cout << "Copied an S\n";
  }

  S(S&&)
  {
    std::cout << "Moved an S\n";
  }

  S() = default;
};

class A {
  S s;

public:
  ~A() = default;
};

int main(int argc, char* argv[])
{
  A a1;

  std::vector<A> vas;

  vas.push_back(A());
}

