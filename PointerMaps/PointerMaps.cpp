#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <random>

struct MyData
{
  static std::unique_ptr<MyData> Create(int x_, int y_, int z_)
  {
    std::unique_ptr<MyData> p(new MyData());
    p->x = x_;
    p->y = y_;
    p->z = z_;
    return p;
  }

  MyData()
  {
  }

  int x = 0;
  int y = 0;
  int z = 0;
};

struct DumbHash
{
  template <typename T>
  size_t operator()(const T&) const
  {
    return 0;
  }
};


std::uniform_int_distribution<int> distribution(0, 99);
std::mt19937 engine;
auto generator = std::bind(distribution, engine);

template <typename Map_T>
void PrintAddedValues(Map_T& map, int N)
{
  for (int i = 0; i < N; ++i)
  {
    map.clear();
    std::vector<std::unique_ptr<MyData>> vData;

    std::vector<int> v(generator());
    vData.push_back(MyData::Create(1, 2, 3));
    v.resize(generator() + v.size());
    vData.push_back(MyData::Create(4, 2, 0));
    v.resize(generator() + v.size());
    vData.push_back(MyData::Create(6, 7, 3));
    v.resize(generator() + v.size());
    vData.push_back(MyData::Create(3, 2, 9));
    v.resize(generator() + v.size());
    vData.push_back(MyData::Create(3, 2, 3));
    v.resize(generator() + v.size());
    vData.push_back(MyData::Create(0, 2, 0));
    v.resize(generator() + v.size());
    vData.push_back(MyData::Create(9, 7, 3));
    v.resize(generator() + v.size());
    vData.push_back(MyData::Create(8, 2, 9));
    v.resize(generator() + v.size());


    for (auto& ptr : vData)
    {
      map[ptr.get()] = map.size();
    }

    for (auto& kv : map)
    {
      std::cout << kv.second << " ";
    }
    std::cout << std::endl;
  }
}

int main(int argc, char* argv[])
{
  int repeats = 12;

  std::cout << "Tree Map" << std::endl;
  std::map<MyData*, double> tree_map_data_value;
  PrintAddedValues(tree_map_data_value, repeats);
  std::cout << std::endl;

  std::cout << "Hash Map" << std::endl;
  std::unordered_map<MyData*, double, DumbHash> hash_map_data_value;
  PrintAddedValues(hash_map_data_value, repeats);
}
