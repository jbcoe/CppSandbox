// Howard Hinnant's  example from n4002
// "Cleaning up noexcept in the libraries"
// Minor modifications made

#include <iostream>
#include <string>
#include <vector>
#include <Common/Timer.h>

template <bool IS_NO_EXCEPT>
class A
{
private:
  std::string s_;

public:
  A() : s_('a', 100) {}

  A(const A&) = default;

  A(A&& a) noexcept(IS_NO_EXCEPT) : s_(std::move(a.s_)) {}
};

template <bool B>
void TimeEmplaceBack(size_t count, const char* message)
{
  std::vector<A<B>> myAs(count);

  {
    auto t = make_timer(message);
    myAs.emplace_back();
  }
}

int main(int argc, char* argv[])
{
  auto count = argc == 1 ? 1000000 : std::stoi(argv[1]);
  TimeEmplaceBack<true>(count, "Emplace back with move");
  TimeEmplaceBack<false>(count, "Emplace back without move");
}
