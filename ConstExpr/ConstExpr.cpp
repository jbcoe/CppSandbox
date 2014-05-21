#include <iostream>

constexpr double GetG() { return 9.81; }

constexpr int GetMyAge() { return 32; }

int main(int argc, char* argv[])
{
  double myValues[GetMyAge()];

  static_assert(GetG() == 9.81, "Get G does not return 9.81");
}
