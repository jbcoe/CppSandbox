#include <cstdio>
#include <initializer_list>
#include <type_traits>

template <typename T>
class array_view
{
  size_t size_ = 0;
  const T* data_ = nullptr;

public:
  array_view(const std::initializer_list<T>& l)
      : size_(l.size()), data_(l.begin())
  {
  }

  const T* data() const
  {
    return data_;
  }

  size_t size() const
  {
    return size_;
  }

  const T* begin() const
  {
    return data_;
  }

  const T* end() const
  {
    return data_ + size_;
  }
};

#if defined(PASS_BY_VALUE)
void print_range(array_view<int> r);
#elif defined(PASS_BY_REF)
void print_range(const array_view<int>& r);
#else
#error Define PASS_BY_VALUE or PASS_BY_REF
#endif

int main(int argc, char* argv[])
{
  array_view<int> ints = {0, 1, 2, 3, 4, 5, 6};
  print_range(ints);
}

