#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
#include <random>
#include <Common/Timer.h>


template <typename Collection_t>
void RandomOrderedFill(Collection_t& collection, const int maxElement,
                       const size_t count)
{
  std::uniform_int_distribution<int> distribution(0, maxElement);
  std::mt19937 engine;
  auto generator = std::bind(distribution, engine);

  for (size_t i{}; i < count; ++i)
  {
    auto v = generator();
    auto find_insertion_point =
        std::find_if(collection.begin(), collection.end(), [=](int x)
    { return x >= v; });
    if (find_insertion_point == collection.end())
      collection.push_back(v);
    else if (find_insertion_point != collection.begin())
      collection.insert(--find_insertion_point, v);
    else
      collection.insert(collection.begin(), v);
  }
}

int main(int argc, char* argv[])
{
  size_t multiplier = 1;
  if (argc > 1)
  {
    multiplier = std::stoi(argv[1]);
  }

  size_t numbers{100000 * multiplier};

  {
    std::vector<int> ints;
    auto t = make_timer("vector");
    RandomOrderedFill(ints, 100, numbers);
  }

  {
    std::list<int> ints;
    auto t = make_timer("list");
    RandomOrderedFill(ints, 100, numbers);
  }
}
