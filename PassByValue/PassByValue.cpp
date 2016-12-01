#include <cstdio>
#include <initializer_list>
#include <type_traits>

template <typename T>
class array_view {
  size_t size_ = 0;
  const T* data_ = nullptr;

public:
  array_view(const std::initializer_list<T>& l)
      : size_(l.size()), data_(l.begin()) {}

  const T* data() const { return data_; }
  size_t size() const { return size_; }
  const T* begin() const { return data_; }
  const T* end() const { return data_ + size_; }
};
#if !defined(VALUE_SIZE)
#define VALUE_SIZE 32
#endif
template <typename T, bool=(sizeof(T)<=VALUE_SIZE)> struct arg;

template <typename T>
struct arg<T, false> { using type = const T&; };
  
template <typename T>
struct arg<T, true> { using type = T; };

template <typename T>
using arg_t = typename arg<T>::type;

void print_range(arg_t<array_view<int>>);               

int main() {
  auto ints = {0, 1, 2, 3, 4, 5, 6};
  print_range(ints);
  return 0;
}