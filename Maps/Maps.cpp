#include <iostream>
#include <random>
#include <Common/Timer.h>
#include <map>
#include <vector>

template <typename K, typename V>
class FlatMap
{
  using KeyValue = std::pair<K, V>;
  using KeyValueData = std::vector<KeyValue>;
  using iterator = typename KeyValueData::iterator;
  using const_iterator = typename KeyValueData::const_iterator;

private:
  KeyValueData data_;

public:
  const_iterator find(const K& k) const
  {

    auto find_it = std::lower_bound(data_.begin(), data_.end(), k,
                                    [](const auto& kv, const auto& k)
                                    {
                                      return kv.first < k;
                                    });
    if (find_it == data_.end() || find_it->first != k)
    {
      return data_.end();
    }
    return find_it;
  }

  std::pair<iterator, bool> insert(const KeyValue& kv)
  {
    iterator find_it = std::lower_bound(data_.begin(), data_.end(), kv.first,
                                        [](const auto& kv, const auto& v)
                                        {
                                          return kv.first < v;
                                        });
    if (find_it == data_.end())
    {
      data_.push_back(kv);
      auto end_it = data_.end();
      --end_it;
      return std::make_pair(end_it, true);
    }
    if (find_it->first == kv.first)
    {
      return std::make_pair(data_.end(), false);
    }
    data_.insert(find_it, kv);
    return std::make_pair(find_it, true);
  }
};

int main(int argc, char* argv[])
{
  const int BIG_NUMBER = 1E6;

  FlatMap<int, double> flatMappedRandomNumbers;

  std::uniform_real_distribution<double> value_distribution(-1,1);
  std::uniform_int_distribution<int> key_distribution(0, BIG_NUMBER/100);
  std::mt19937 engine;

  auto value_generator = [&]()
  {
    return value_distribution(engine);
  };
  
  auto key_generator = [&]()
  {
    return key_distribution(engine);
  };

  {
    auto t = make_timer("Flat map");
    for (int i = 0; i < BIG_NUMBER; ++i)
    {
      flatMappedRandomNumbers.insert(std::make_pair(i, generator()));
    }
  }


  std::map<int, double> stdMappedRandomNumbers;
  {
    auto t = make_timer("std::map");
    for (int i = 0; i < BIG_NUMBER; ++i)
    {
      stdMappedRandomNumbers.insert(std::make_pair(i, generator()));
    }
  }
}

