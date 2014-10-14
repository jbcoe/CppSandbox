#include <iostream>

struct Base
{
  virtual void DoThing() const final {}
};

struct Derived final : Base
{
  void DoThing() const final {}
};

struct FurtherDerived : Derived
{
};

int main(int argc, char* argv[]) {}
