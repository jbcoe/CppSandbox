#include <cassert>
#include <iostream>

constexpr size_t INITIAL_CAPACITY = 10;
constexpr size_t EXPANSION_FACTOR = 2;

template <typename T>
struct buffer
{
  size_t size_ = 0;
  size_t capacity_ = 0;
  std::unique_ptr<T[]> data_;

  buffer(size_t capacity)
      : capacity_(capacity), data_(std::make_unique<T[]>(capacity))
  {
  }

  void expand(size_t n)
  {
    auto new_data = std::make_unique<T[]>(n);
    for(size_t i=0; i<size_; ++i)
    {
      new_data[i] = std::move(data_[i]);
    }
    data_ = std::move(new_data);
    capacity_ = n;
  }

  T& push_back(T t)
  {
    if (size_ < capacity_)
    {
      data_[size_] = std::move(t);
      ++size_;
      return data_[size_ - 1];
    }
    else
    {
      expand(capacity_ * EXPANSION_FACTOR);
      return push_back(std::move(t));
    }
  }
};

template <typename T>
class vector
{
  std::unique_ptr<buffer<T>> buffer_;

public:
  size_t size() const noexcept
  {
    return buffer_ ? buffer_->size_ : 0;
  }

  const T* data() const noexcept
  {
    return buffer_ ? buffer_->data_ : nullptr;
  }

  T* data() noexcept
  {
    return buffer_ ? buffer_->data_ : nullptr;
  }

  const T& operator[](size_t i) const noexcept
  {
    assert(buffer_ && i < buffer_->size_);
    return buffer_->data_[i];
  }

  T& operator[](size_t i) noexcept
  {
    assert(buffer_ && i < buffer_->size_);
    return buffer_->data_[i];
  }

  const T* begin() const noexcept
  {
    return buffer_ ? &buffer_->data_[0] : nullptr;
  }

  const T* end() const noexcept
  {
    return buffer_ ? &buffer_->data_[buffer_->size_] : nullptr;
  }

  T* begin() noexcept
  {
    return buffer_ ? &buffer_->data_[0] : nullptr;
  }

  T* end() noexcept
  {
    return buffer_ ? &buffer_->data_[buffer_->size_] : nullptr;
  }

  T& push_back(T t) noexcept
  {
    if (!buffer_) buffer_ = std::make_unique<buffer<T>>(INITIAL_CAPACITY);
    return buffer_->push_back(std::move(t));
  }
};

int main(int argc, char* argv[])
{
  vector<int> vs;
  static_assert(sizeof(vs) == sizeof(void*),"");

  vs.push_back(10);
  assert(vs.size() == 1);
  assert(vs[0] == 10);
}

