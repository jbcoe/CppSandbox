#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char* argv[])
{
  std::vector<int> some_list = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  int count = 0;
  int sum = 0;

  std::for_each(some_list.begin(), some_list.end(), [&count, &sum](int x)
  {
    sum += x;
    ++count;
  });

  std::cout << "Sum: " << sum << " count: " << count << std::endl;
}
