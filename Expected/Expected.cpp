// Based on a sub-set of https://isocpp.org/files/papers/n4109.pdf
#include <iostream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <cassert>

struct Unexpected_T
{
};
constexpr Unexpected_T unexpected{};

// used internally co control Data_ ctor
// no real need for a constexpr instance
struct Expected_T {};

template <typename T>
class Expected
{
  union Data
  {
    std::exception_ptr e_;
    T t_;

    // Don't initialise either member
    Data() {}
    // Construct the 'e_' member
    template<typename ...Args>
    Data( Unexpected_T, Args&&...args )
     : e_{ std::forward<Args>(args)... }
    {}
    // Construct the 't_' member
    template<typename ...Args>
    Data( Expected_T, Args&&...args )
     : t_{ std::forward<Args>(args)... }
    {}
    ~Data() {}
  } data_;

  bool hasData_;

  template <typename U>
  friend class Expected;

  Expected() noexcept
    : data_ { Unexpected_T{}, nullptr }
    , hasData_(false)
  {}

public:
  ~Expected()
  {
    if( hasData_ )
    {
      data_.t_.~T();
    }
    else
    {
      data_.e_.~exception_ptr();
    }
  }
  template <typename E>
  Expected(Unexpected_T, E&& e) noexcept // this most probably isn't noexcept in the general case
    : data_{ Unexpected_T{}, std::make_exception_ptr(std::forward<E>(e)) }
    , hasData_(false)
  {
  }

  Expected(T&& t)
    : data_{ Expected_T{}, std::move(t) }
    , hasData_(true)
  {}

  Expected(const T& t)
    : data_{ Expected_T{}, t }
    , hasData_(true)
  {}

  Expected(Expected<T>&& x) : hasData_(x.hasData_)
  // nb: here, neither 'e_' nor 't_' is initialised
  {
    if (x.hasData_)
    {
      ::new (std::addressof(data_.t_)) T { std::move(x.data_.t_) };
    }
    else
    {
      ::new (std::addressof(data_.e_)) std::exception_ptr { std::move(x.data_.e_) };
    }
  }

  explicit operator bool() const noexcept { return hasData_; }

  T& operator*()
  {
    if (hasData_)
    {
      return data_.t_;
    }
    std::rethrow_exception(data_.e_);
  }

  const T& operator*() const
  {
    if (hasData_)
    {
      return data_.t_;
    }
    std::rethrow_exception(data_.e_);
  }

  template <typename U>
  Expected<U> as_unexpected() noexcept
  {
    assert( !hasData_ );
    Expected<U> u;
    u.data_.e_ = data_.e_;
    return std::move(u);
  }
  
  template <typename U, typename E>
  Expected<U> as_unexpected(E&& e) noexcept
  {
    assert( !hasData_ );
    Expected<U> u;
    try
    {
      try
      {
        std::rethrow_exception(data_.e_);
      }
      catch (...)
      {
        std::throw_with_nested(std::forward<E>(e));
      }
    }
    catch (...)
    {
      u.data_.e_ = std::current_exception();
    }
    return std::move(u);
  }
};

template <typename T>
auto make_expected(T&& t)
{
  return Expected<std::decay_t<T>>(std::forward<T>(t));
}

template <typename T, typename E>
auto make_unexpected(E&& e)
{
  return Expected<std::decay_t<T>>(unexpected, std::forward<E>(e));
}

///////////////////////////////////////////////////////////////////////////////
// Main and associated test functions
///////////////////////////////////////////////////////////////////////////////

Expected<int> ToInt(const std::string& s) noexcept
{
  try
  {
    return std::stoi(s);  
  }
  catch(...)
  {
    std::stringstream ss;
    ss << "Failed to convert \"" << s << "\" to int";
    return make_unexpected<int>(std::runtime_error(ss.str().c_str()));
  }
}

Expected<std::pair<int,int>> GetFraction(const std::string& s, const std::string& t) noexcept
{
 auto eInt_s = ToInt(s);
 auto eInt_t = ToInt(t);

 if (!eInt_s) return eInt_s.as_unexpected<std::pair<int, int>>(std::runtime_error("Failed to convert numerator"));

 if (!eInt_t) return eInt_t.as_unexpected<std::pair<int, int>>(std::runtime_error("Failed to convert denominator"));

 return std::make_pair(*eInt_s, *eInt_t);
}

void PrintFraction(const std::pair<int, int>& f)
{
  std::cout << f.first << "/" << f.second <<"\n";
}

void PrintException(const std::exception& e, int level=0)
{
  std::cerr << std::string(level, ' ') << e.what() << '\n';
  try
  {
    std::rethrow_if_nested(e);
  }
  catch(const std::exception& e)
  {          
    PrintException(e, level+1);
  }
  catch (...){}
}

int main(int argc, char* argv[])
{
  try
  {
    PrintFraction(*GetFraction("5","6"));
    PrintFraction(*GetFraction("5","foo"));
  }
  catch (const std::runtime_error& e)
  {
    PrintException(e);
  }
}  

