#include <iostream>
#include <vector>

template <typename T, typename F>
class CheckedArrayView
{
  T* data_;
  size_t size_;

public:
  
  struct CheckViolated: public std::runtime_error
  {
    CheckViolated() : std::runtime_error("Checked condition violated")
    {
    }
  };
  
  using const_iterator = const T*;

  template <typename C>
  CheckedArrayView(C& c)
      : data_(c.data()), size_(c.size())
  {
    if ( ! F()(begin(),end()) ) { throw CheckViolated(); }
  }

  CheckedArrayView(T* data, size_t size) : data_(data), size_(size)
  {
    if ( ! F()(begin(),end()) ) { throw CheckViolated(); }

    if (!data_)
    {
      size_ = 0;
    }
  }

  bool empty() const
  {
    return size_ == 0;
  }

  size_t size() const
  {
    return size_;
  }

  const T* data() const
  {
    return data_;
  }

  const_iterator begin() const
  {
    return data_;
  }

  const_iterator end() const
  {
    return data_ + size_;
  }

  const_iterator cbegin() const
  {
    return data_;
  }

  const_iterator cend() const
  {
    return data_ + size_;
  }
};

template <typename F, typename C,
          typename U = typename std::enable_if<!std::is_array<C>::value>::type>
auto make_checked_array_view(C& c)
{
  using data_t = decltype(c.data());
  using value_t = std::remove_reference_t<decltype(*std::declval<data_t>())>;
  return CheckedArrayView<value_t, F>(c);
}
/*
template <typename F, typename T>
auto make_checked_array_view(T* t, size_t size)
{
  return CheckedArrayView<T, F>(t, size);
}

template <typename F, typename T, size_t N>
auto make_checked_array_view(T(&t)[N])
{
  return CheckedArrayView<T, F>(t, N);
}
*/


struct Positive
{
  template <typename I>
  bool operator()(I begin, I end)
  {
    while (begin != end)
    {
      if (*begin < 0)
      {
        return false;
      }
      ++begin;
    }
    return true;
  }
};

struct Even
{
  template <typename I>
  bool operator()(I begin, I end)
  {
    while (begin != end)
    {
      if (*begin %2 != 0)
      {
        return false;
      }
      ++begin;
    }
    return true;
  }
};

void f(const CheckedArrayView<int, Even>& c)
{
}

int main(int argc, char* argv[])
{
  std::vector<int> ints = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  auto positive_ints = make_checked_array_view<Positive>(ints);
  for (const auto i : positive_ints)
  {
    std::cout << i << " ";
  }
  std::cout << std::endl;
  
  try
  {
    f(ints);
  }
  catch(const std::runtime_error e)
  {
    std::cerr << e.what() << "\n";
  }
}
