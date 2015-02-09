#include <iostream>
#include <cassert>
#include <iomanip>

template <typename T>
class deep_ptr
{
  public:
    deep_ptr() : i_() {}

    deep_ptr(std::nullptr_t) : i_(nullptr) {}

    template <typename U>
    deep_ptr(U* u)
        : i_(u ? new inner_impl<U>(*u) : nullptr)
    {
    }

    ~deep_ptr() { delete i_; }

    deep_ptr(const deep_ptr& p) : i_(p.i_ ? p.i_->copy() : nullptr) {}

    deep_ptr& operator=(const deep_ptr& p)
    {
      if (!p.i_)
      {
        i_ = nullptr;
      }
      else
      {
        i_ = p.i_->copy();
      }
    }

    deep_ptr(deep_ptr&& p) : i_(p.i_) { p.i_ = nullptr; }

    deep_ptr& operator=(deep_ptr&& p)
    {
      i_ = p.i_;
      p.i_ = nullptr;
    }

    T* operator->() const { return get(); }

    T* get() const
    {
      if (i_)
      {
        return *i_;
      }
      return nullptr;
    }

    T& operator*() const { return *static_cast<T*>(*i_); }

    T* operator->() { return get(); }

    T* get()
    {
      if (i_)
      {
        return *i_;
      }
      return nullptr;
    }

    T& operator*(){ return *static_cast<T*>(*i_); }

  private:
    struct inner
    { 
      virtual inner* copy() const = 0;
      
      virtual operator const T*() const  = 0;
      
      virtual operator T*() = 0;

      virtual ~inner() {}
    };
    
    inner* i_;

    template <typename U>
    struct inner_impl : inner
    {
      inner_impl(const U& u) : u_(u) {}

      inner_impl* copy() const override { return new inner_impl(u_); }

      operator const T*() const override { return &u_; }

      operator T*() override { return &u_; }

      U u_;
    };
};

struct IA
{
  virtual ~IA() {}
  virtual const char* name() const = 0;
};

struct CA : IA
{
  const char* name() const override { return "CA"; }
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
}

