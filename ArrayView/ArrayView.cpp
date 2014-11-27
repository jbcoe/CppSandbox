#include <iostream>
#include <vector>

template <typename T>
class ArrayView
{
  T* data_;
  size_t size_;

public:

  using iterator = T*;
  
  using const_iterator = const T*;
  
  template <typename C>
  ArrayView(C& c)
      : data_(c.data()), size_(c.size())
  {
  }

  ArrayView(T* data, size_t size) : data_(data), size_(size)
  {
    if (!data_)
    {
      size_ = 0;
    }
  }

  bool empty() const { return size_==0; }
  
  size_t size() const { return size_; }
  
  T* data() { return data_; }
  
  const T* data() const { return data_; }

  iterator begin() { return data_; }
  
  iterator end() { return data_+size_; }
  
  const_iterator begin() const { return data_; }
  
  const_iterator end() const { return data_+size_; }
  
  const_iterator cbegin() const { return data_; }
  
  const_iterator cend() const { return data_+size_; }
};

template<typename C>
auto make_array_view(C& c)
{
  using data_t = decltype(c.data());
  using value_t = std::remove_reference_t<decltype(*std::declval<data_t>())>;
  return ArrayView<value_t>(c);
}

template<typename T>
auto make_array_view(T* t, size_t size)
{
  return ArrayView<T>(t,size);
}

int main(int argc, char* argv[]) 
{
  std::vector<int> ints = {0,1,2,3,4,5,6,7,8,9};
  auto view_ints = make_array_view(ints);
  for (const auto i : view_ints)
  {
    std::cout << i << " ";
  }
  std::cout << std::endl;
  
  double doubles[] = {2.81718, 3.14159};
  auto view_doubles = make_array_view(doubles, 2);
  for (const auto d : view_doubles)
  {
    std::cout << d << " ";
  }
  std::cout << std::endl;
}

