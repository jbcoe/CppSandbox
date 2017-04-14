#include <algorithm>
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
  std::istream_iterator<int> in(std::cin);
  std::istream_iterator<int> end;

  std::vector<int> xs(in, end);

  std::make_heap(xs.begin(), xs.end());
  auto last = xs.end();
  while (last != xs.begin())
  {
    std::pop_heap(xs.begin(), last);
    --last;
  }

  std::copy(xs.begin(), xs.end(), std::ostream_iterator<int>(std::cout, " "));
}

