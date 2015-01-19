#include <iostream>
#include <cassert>
#include <iomanip>

template <typename T>
class deep_ptr
{
  public:
    deep_ptr() : i_() {}

    deep_ptr(std::nullptr_t) : i_(nullptr) {}
    
    template<typename U>
    deep_ptr(U* u) : i_(new inner_impl<U>(u)) {}

    ~deep_ptr() { delete i_; }

    deep_ptr(const deep_ptr& p) : i_(p.i_->copy()) {}

    deep_ptr& operator=(const deep_ptr& p) { i_ = p.i_->copy(); }

    deep_ptr(deep_ptr&& p) : i_(p.i_) { p.i_ = nullptr; }

    deep_ptr& operator=(deep_ptr&& p)
    {
      i_ = p.i_;
      p.i_ = nullptr;
    }

    const T* operator->() const { return get(); }

    const T* get() const
    {
      if (i_)
      {
        return *i_;
      }
      return nullptr;
    }

    const T& operator*() const { return *static_cast<T*>(*i_); }

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
      inner(T* t) : t_(t) {}

      inner* copy() const
      {
        if(!t_) { return nullptr; }
        return do_copy();
      }

      operator const T*() const { return t_; } 
      operator T*() { return t_; } 
      virtual ~inner() { delete t_; }

    private:
      virtual inner* do_copy() const = 0;

    protected:
      T* t_;
    };
    
    inner* i_;

    template<typename U>
    struct inner_impl : inner
    {
      inner_impl(U* u) : inner(u) {}
      inner_impl* do_copy() const override
      {
        assert(inner::t_);
        const U& u = static_cast<U&>(*inner::t_);
        return new inner_impl(new U(u));
      }
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

