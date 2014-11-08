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

// used internally to control Data_ ctor
// no real need for a constexpr instance
struct Expected_T
{
};

template <typename T>
class Expected
{
  bool hasData_;
  
  union Data
  {
    std::exception_ptr e_;
    T t_;

    // Zero initialise the 'e_' member
    Data() : e_{} {}
    // Construct the 'e_' member
    template <typename... Args>
    Data(Unexpected_T, Args&&... args)
        : e_{std::forward<Args>(args)...}
    {
    }
    // Construct the 't_' member
    template <typename... Args>
    Data(Expected_T, Args&&... args)
        : t_{std::forward<Args>(args)...}
    {
    }
    ~Data() {}
  } data_;

  template <typename U>
  friend class Expected;

  Expected() noexcept : hasData_(false), data_{Unexpected_T{}, nullptr} {}

public:
  ~Expected()
  {
    if (hasData_)
    {
      data_.t_. ~T();
    }
    else
    {
      data_.e_. ~exception_ptr();
    }
  }
  template <typename U>
  Expected(Unexpected_T, U&& u) noexcept try
      : hasData_(false),
        data_(Unexpected_T{}, std::make_exception_ptr(std::forward<U>(u)))

  {
  }
  catch(...)
  {
    ::new (std::addressof(data_.e_)) std::exception_ptr{};
    data_.e_ = std::current_exception();
  }

  Expected(T&& t) noexcept try : hasData_(true),
                                 data_(Expected_T{}, std::move(t))

  {
  }
  catch(...)
  {
    ::new (std::addressof(data_.e_)) std::exception_ptr{};
    data_.e_ = std::current_exception();
  }

  Expected(const T& t) noexcept try : hasData_(true), data_(Expected_T{}, t) 
  {
  }
  catch(...)
  {
    hasData_ = false;
    ::new (std::addressof(data_.e_)) std::exception_ptr{};
    data_.e_ = std::current_exception();
  }

  Expected(const Expected<T>& x) noexcept : hasData_(x.hasData_)
  {
    try
    {
      if (x.hasData_)
      {
        ::new (std::addressof(data_.t_)) T{x.data_.t_};
      }
      else
      {
        ::new (std::addressof(data_.e_))
            std::exception_ptr{x.data_.e_};
      }
    }
    catch(...)
    {
      hasData_ = false;
      ::new (std::addressof(data_.e_)) std::exception_ptr{};
      data_.e_ = std::current_exception();
    }
  }
  
  Expected(Expected<T>&& x) noexcept : hasData_(x.hasData_)
  {
    try
    {
      if (x.hasData_)
      {
        ::new (std::addressof(data_.t_)) T{std::move(x.data_.t_)};
      }
      else
      {
        ::new (std::addressof(data_.e_)) std::exception_ptr{std::move(x.data_.e_)};
      }
    }
    catch(...)
    {
      hasData_ = false;
      ::new (std::addressof(data_.e_)) std::exception_ptr{};
      data_.e_ = std::current_exception();
    }
  }

  // TODO: if internal assignment throws, = can leave Expected in a bad state 
  Expected& operator = (const Expected& ) = delete;
  Expected& operator = (Expected&& ) = delete;

  explicit operator bool() const noexcept { return hasData_; }

  T& operator*()
  {
    if (hasData_)
    {
      return data_.t_;
    }
    assert(data_.e_);
    std::rethrow_exception(data_.e_);
  }

  const T& operator*() const
  {
    if (hasData_)
    {
      return data_.t_;
    }
    assert(data_.e_);
    std::rethrow_exception(data_.e_);
  }

  template <typename U>
  Expected<U> as_unexpected() noexcept
  {
    assert(!hasData_);
    assert(data_.e_);
    Expected<U> u;
    u.data_.e_ = data_.e_;
    return u;
  }

  template <typename U, typename E>
  Expected<U> as_unexpected(E&& e) noexcept
  {
    assert(!hasData_);
    assert(data_.e_);
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
    return u;
  }
};

template <typename T>
inline auto make_expected(T&& t) noexcept
{
  return Expected<std::decay_t<T>>(std::forward<T>(t));
}

template <typename T, typename E>
inline auto make_unexpected(E&& e) noexcept
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
  catch (...)
  {
    std::ostringstream ss;
    ss << "Failed to convert \"" << s << "\" to int";
    return make_unexpected<int>(std::runtime_error(ss.str()));
  }
}

Expected<std::pair<int, int>> GetFraction(const std::string& s,
                                          const std::string& t) noexcept
{
  auto eInt_s = ToInt(s);
  auto eInt_t = ToInt(t);

  if (!eInt_s)
  {
    return eInt_s.as_unexpected<std::pair<int, int>>(
        std::runtime_error("Failed to convert numerator"));
  }

  if (!eInt_t)
  {
    return eInt_t.as_unexpected<std::pair<int, int>>(
        std::runtime_error("Failed to convert denominator"));
  }

  return std::make_pair(*eInt_s, *eInt_t);
}

void PrintFraction(const std::pair<int, int>& f)
{
  std::cout << f.first << "/" << f.second << "\n";
}

void PrintException(const std::exception& e, int level = 0)
{
  std::cerr << std::string(level, ' ') << e.what() << '\n';
  try
  {
    std::rethrow_if_nested(e);
  }
  catch (const std::exception& e)
  {
    PrintException(e, level + 1);
  }
  catch (...)
  {
  }
}

int main(int argc, char* argv[])
{
  try
  {
    std::cout << "Try divide \"5\" by \"6\"\n";
    PrintFraction(*GetFraction("5", "6"));
    std::cout << "Try divide \"5\" by \"foo\"\n";
    PrintFraction(*GetFraction("5", "foo"));
  }
  catch (const std::runtime_error& e)
  {
    PrintException(e);
  }
}
