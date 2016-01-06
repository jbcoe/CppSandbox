#include <iostream>
#include <array>
#include <cassert>
#include <iomanip>

template <typename T>
class deep_ptr
{
  using buffer_t = std::array<char, 2 * sizeof(void*)>;
  buffer_t buffer_;
  bool engaged_ = false;

  struct inner
  {
    virtual void copy(buffer_t& buffer) const = 0;

    virtual const T* get() const = 0;

    virtual T* get() = 0;

    virtual ~inner() = default;
  };

  template <typename U>
  struct inner_impl : inner
  {
    inner_impl(const inner_impl&) = delete;
    inner_impl& operator=(const inner_impl&) = delete;

    inner_impl(U* u) : u_(u)
    {
    }

    void copy(buffer_t& buffer) const override
    {
      static_assert(sizeof(buffer_t) == sizeof(inner_impl), "size-mismatch");
      new (buffer.data()) inner_impl(new U(*u_));
    }

    const T* get() const override
    {
      return u_;
    }

    T* get() override
    {
      return u_;
    }

    ~inner_impl()
    {
      delete u_;
    }

    U* u_;
  };

public:
  deep_ptr()
  {
    buffer_.fill(0);
  }

  deep_ptr(std::nullptr_t) : deep_ptr()
  {
  }

  template <typename U>
  deep_ptr(U* u)
  {
    if (!u)
    {
      engaged_ = false;
      return;
    }

    new (buffer_.data()) inner_impl<U>(u);
    engaged_ = true;
  }

  ~deep_ptr()
  {
    if (engaged_)
    {
      reinterpret_cast<inner*>(buffer_.data())->~inner();
    }
  }

  deep_ptr(const deep_ptr& p)
  {
    if (!p.engaged_) return;
    reinterpret_cast<const inner*>(p.buffer_.data())->copy(buffer_);
    engaged_ = true;
  }

  deep_ptr& operator=(const deep_ptr& p)
  {
    if (!p.engaged_)
    {
      if (engaged_)
      {
        reinterpret_cast<inner*>(buffer_.data())->~inner();
      }
      engaged_ = false;
    }
    else
    {
      reinterpret_cast<inner*>(p.buffer_)->copy(buffer_.data());
      engaged_ = true;
    }
  }

  deep_ptr(deep_ptr&& p)
  {
    buffer_ = p.buffer_;
    engaged_ = true;
    p.engaged_ = false;
    p.buffer_.fill(0);
  }

  deep_ptr& operator=(deep_ptr&& p)
  {
    engaged_ = p.engaged_;
    if (p.engaged_)
    {
      buffer_ = p.buffer_;
    }
    p.engaged_ = false;
    p.buffer.fill(0);
    return *this;
  }

  const T* operator->() const
  {
    return get();
  }

  const T* get() const
  {
    if (!engaged_)
    {
      return nullptr;
    }
    return reinterpret_cast<const inner*>(buffer_)->get();
  }

  const T& operator*() const
  {
    assert(engaged_);
    return *get();
  }

  T* operator->()
  {
    return get();
  }

  T* get()
  {
    if (!engaged_)
    {
      return nullptr;
    }
    return reinterpret_cast<inner*>(buffer_.data())->get();
  }

  T& operator*()
  {
    assert(engaged_);
    return *get();
  }
};

struct IA
{
  virtual ~IA()
  {
  }
  virtual const char* name() const = 0;
};

struct CA : IA
{
  const char* name() const override
  {
    return "CA";
  }
};

int main(int argc, char* argv[])
{
  deep_ptr<IA> pA(new CA());
  auto pA_copy = pA;
  std::cout << std::hex << pA.get() << std::endl;
  std::cout << std::hex << pA_copy.get() << std::endl;
  std::cout << std::endl;

  auto pA_moved = std::move(pA);
  std::cout << std::hex << pA.get() << std::endl;
  std::cout << std::hex << pA_moved.get() << std::endl;
  std::cout << std::endl;
}
