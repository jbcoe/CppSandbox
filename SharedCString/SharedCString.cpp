#include <atomic>
#include <array>
#include <iostream>

class string_view
{
  const char* data_ = nullptr;
  size_t size_ = 0;

public:
  string_view(const char* c, size_t size) : data_(c), size_(size)
  {
    if (!data_) size_ = 0;
  }

  size_t size() const noexcept
  {
    return size_;
  }

  const char* data() const noexcept
  {
    return data_;
  }
};

template <typename T>
class dynamic_array
{
  std::unique_ptr<T[]> data_;
  size_t size_ = 0;

public:
  dynamic_array(size_t size) : size_(size)
  {
    data_ = std::make_unique<T[]>(size_);
    std::fill(data_.get(), data_.get() + size_, T{});
  }

  dynamic_array(const dynamic_array& d) : size_(d.size_)
  {
    data_ = std::make_unique<T[]>(size_);
    std::copy(d.data_.get(), d.data_.get() + size_, data_.get());
  }

  dynamic_array operator=(const dynamic_array& d)
  {
    auto tmp = std::make_unique<T[]>(size_);
    std::copy(d.data_.get(), d.data_.get() + size_, tmp.get());
    size_ = d.size_;
    data_ = std::move(tmp);
  }

  dynamic_array(dynamic_array&& d) noexcept : data_(std::move(d.data_)), size_(d.size_)
  {
    d.size_ = 0;
  }

  dynamic_array operator=(dynamic_array&& d) noexcept
  {
    reset();
    swap(d);
  }

  void swap(dynamic_array<T>& d) noexcept
  {
    swap(d.data_, data_);
    swap(d.size_, size_);
  }

  void reset() noexcept
  {
    data_.reset();
    size_ = 0;
  }

  size_t size() const noexcept
  {
    return size_;
  }

  const T* data() const noexcept
  {
    return data_;
  }

  T* data() noexcept
  {
    return data_.get();
  }
};

template <size_t N>
class shared_cstring
{
  bool is_big_ = false;
  size_t size_ = 0;
  struct big_data
  {
    big_data() = default;
    dynamic_array<char>* data_ = nullptr;
    std::atomic<size_t>* ref_count_ = nullptr;
  };

  union data {
    std::array<char, N> small_data_;
    big_data big_data_;
    data() : small_data_{}
    {
      small_data_.fill(0);
    }
  } data_;

  shared_cstring(const char* d, size_t size) : size_(size)
  {
    if (size_ > N)
    {
      is_big_ = true;
      new (&data_.big_data_) big_data();
      data_.big_data_.data_ = new dynamic_array<char>(size_);
      data_.big_data_.ref_count_ = new std::atomic<size_t>(1);
    }
    std::copy(d, d + size_, mutable_data());
  }

  char* mutable_data()
  {
    return is_big_ ? data_.big_data_.data_->data() : data_.small_data_.data();
  }

public:
  shared_cstring()
  {
  }

  template <size_t S>
  shared_cstring(const char (&a)[S]) : shared_cstring(a, S)
  {
  }

  shared_cstring(string_view v) : shared_cstring(v.data(), v.size())
  {
  }

  shared_cstring(const shared_cstring& s) : size_(s.size)
  {
    if (s.is_big_)
    {
      new (data_.big_data_) big_data();
      data_.big_data_.data_ = s.data_.big_data_.data_;
      data_.big_data_.ref_count_ = s.data_.big_data_.ref_count_;
      ++(*data_.big_data_.ref_count_);
      is_big_ = true;
    }
    else
    {
      data_.small_data_ = s.data_.small_data_;
    }
  }

  shared_cstring(shared_cstring&& s) : size_(s.size)
  {
    if (s.is_big_)
    {
      new (data_.big_data_) big_data();
      std::swap(data_.big_data_.data_, s.data_.big_data_.data_);
      std::swap(data_.big_data_.ref_count_, s.data_.big_data_.ref_count_);
      std::swap(is_big_, s.is_big_);
    }
    else
    {
      data_.small_data_ = std::move(s.data_.small_data_);
    }
    s.size_ = 0;
  }

  size_t size() const noexcept
  {
    return size_;
  }

  const char* data() const noexcept
  {
    return is_big_ ? data_.big_data_.data_->data() : data_.small_data_.data();
  }

  operator string_view() const
  {
    return is_big_ ? string_view(data_.big_data_->data(), size_)
                   : string_view(data_.small_data_.data(), size_);
  }

  ~shared_cstring()
  {
    if (is_big_)
    {
      if (data_.big_data_.ref_count_->fetch_sub(1) == 1)
      {
        delete data_.big_data_.data_;
        delete data_.big_data_.ref_count_;
      }
    }
  }
};

template<size_t N, typename OS>
OS& operator << (OS& os, const shared_cstring<N>& c)
{
  auto it = c.data();
  auto end_it = c.data() + c.size();
  while(it!=end_it)
  {
    os << *it;
    ++it;
  }
  return os;
}

int main(int argc, char* argv[])
{
  shared_cstring<16> hi("Hello world");
  shared_cstring<2> bye("Hello world");
  std::cout << hi << '\n';
  std::cout << bye << '\n';
}

