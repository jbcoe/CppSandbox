#include <Common/Timer.h>

int main(int argc, char* argv[])
{
  auto t(make_timer([](long t)
  { std::cout << "Time passed: " << t << std::endl; }));
  auto t2 = make_timer("Build vector");
  Timer t3("Another timer");
  std::vector<int> v(10000000);
  assert(std::find(v.begin(), v.end(), 9) == v.end());
}
