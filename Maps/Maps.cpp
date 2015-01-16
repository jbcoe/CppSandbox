#include <iostream>
#include <random>
#include <Common/Timer.h>
#include <map>
#include <vector>
#include <deque>

template<typename K,typename V>
class IndexedKeyMap
{
  std::vector<K> keys_;
  std::vector<V> values_;

public:
  
  const V* find(const K& k) const
  {
    auto find_it = std::lower_bound(keys_.begin(), keys_.end(), k, [](const auto& k_, const auto& k)
        {
          return k_ < k;
        });
  
    if (find_it == keys_.end() || *find_it != k)
    {
      return nullptr;
    }
    auto index = std::distance(keys_.begin(), find_it);
    return &values_[index];
  }

  bool insert(const std::pair<K,V>& kv)
  {
    auto find_it = std::lower_bound(keys_.begin(), keys_.end(), kv.first,
                                        [](const auto& kv, const auto& k)
                                        {
                                          return kv < k;
                                        });
    if (find_it == keys_.end())
    {
      keys_.push_back(kv.first);
      values_.push_back(kv.second);
      return true;
    }                                      
    if (*find_it == kv.first)
    {
      return false;
    }
    
    auto index = std::distance(keys_.begin(), find_it);
    
    keys_.insert(keys_.begin()+index, kv.first);
    values_.insert(values_.begin()+index, kv.second);
    
    return true;
  }
};

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
  const_iterator end() const { return data_.end(); }

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
  const int BIG_NUMBER = 1E7;

  FlatMap<int, double> flatMappedRandomNumbers;
  std::map<int, double> stdMappedRandomNumbers;
  IndexedKeyMap<int, double> indexedKeyRandomNumbers;

  std::uniform_real_distribution<double> value_distribution(-1, 1);
  std::uniform_int_distribution<int> key_distribution(0, BIG_NUMBER / 100);
  std::mt19937 engine(0);

  auto value_generator = [&]()
  {
    return value_distribution(engine);
  };

  auto key_generator = [&]()
  {
    return key_distribution(engine);
  };

  std::cout << "\nPopulate\n";
  {
    auto t = make_timer("Flat map");
    for (int i = 0; i < BIG_NUMBER; ++i)
    {
      flatMappedRandomNumbers.insert(
          std::make_pair(key_generator(), value_generator()));
    }
  }

  engine.seed(0);
  {
    auto t = make_timer("std::map");
    for (int i = 0; i < BIG_NUMBER; ++i)
    {
      stdMappedRandomNumbers.insert(
          std::make_pair(key_generator(), value_generator()));
    }
  }

  engine.seed(0);
  {
    auto t = make_timer("index map");
    for (int i = 0; i < BIG_NUMBER; ++i)
    {
      indexedKeyRandomNumbers.insert(
          std::make_pair(key_generator(), value_generator()));
    }
  }
  
  std::cout << "\nLookup\n";
  engine.seed(0);
  auto hits_0 = 0;
  {
    auto t = make_timer("Flat map");
    for (int i = 0; i < BIG_NUMBER; ++i)
    {
      if (flatMappedRandomNumbers.find(key_generator()) !=
          flatMappedRandomNumbers.end())
      {
        ++hits_0;
      }
    }
  }
  engine.seed(0);
  auto hits_1 = 0;
  {
    auto t = make_timer("std::map");
    for (int i = 0; i < BIG_NUMBER; ++i)
    {
      if (stdMappedRandomNumbers.find(key_generator()) !=
          stdMappedRandomNumbers.end())
      {
        ++hits_1;
      }
    }
  }

  engine.seed(0);
  auto hits_2 = 0;
  {
    auto t = make_timer("index map");
    for (int i = 0; i < BIG_NUMBER; ++i)
    {
      if (indexedKeyRandomNumbers.find(key_generator()))
      {
        ++hits_2;
      }
    }
  }
  
  return ( hits_0 == hits_1 && hits_1 == hits_2 ) ? 0 : -1;
}

