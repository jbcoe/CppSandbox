// Based on a sub-set of https://isocpp.org/files/papers/n4109.pdf
#include <iostream>
#include <exception>
#include <stdexcept>

struct Unexpected_T
{
};
constexpr Unexpected_T unexpected{};

template <typename T>
class Expected
{
  union Data
  {
    std::exception_ptr e_;
    T t_;

    Data() : e_(nullptr) {}
    ~Data() {}
  } data_;

  bool hasData_;

  template <typename U>
  friend class Expected;

  Expected() noexcept : hasData_(false) {}

public:
  template <typename E>
  Expected(Unexpected_T, E&& e) noexcept : hasData_(false)
  {
    data_.e_ = std::make_exception_ptr(std::forward<E>(e));
  }

  Expected(T&& t) : hasData_(true) { data_.t_ = std::move(t); }

  Expected(const T& t) : hasData_(true) { data_.t_ = t; }

  Expected(Expected<T>&& x) : hasData_(x.hasData_)
  {
    if (x.hasData_)
    {
      data_.t_ = std::move(x.data_.t_);
    }
    else
    {
      data_.e_ = std::move(x.data_.e_);
    }
  }

  explicit operator bool() const noexcept { return hasData_; }

  T& operator*()
  {
    if (hasData_)
    {
      return data_.t_;
    }
    if(data_.e_)
    {
      std::rethrow_exception(data_.e_);
    }
    throw std::logic_error("No data and no exception");
  }

  const T& operator*() const
  {
    if (hasData_)
    {
      return data_.t_;
    }
    if(data_.e_)
    {
      std::rethrow_exception(data_.e_);
    }
    throw std::logic_error("No data and no exception");
  }

  template <typename U>
  Expected<U> as_unexpected() noexcept
  {
    Expected<U> u;
    try
    {
      if(data_.e_) std::rethrow_exception(data_.e_);
      throw std::logic_error("No data or exception set in Expected<T>");
    }
    catch (...)
    {
      u.data_.e_ = std::current_exception();
    }
    return std::move(u);
  }
};

template <typename T, typename E = std::runtime_error>
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
    return make_unexpected<int>(std::runtime_error("Failed to convert value to int"));
  }
}

Expected<std::pair<int,int>> GetFraction(const std::string& s, const std::string& t) noexcept
{
 auto eInt_s = ToInt(s);
 auto eInt_t = ToInt(t);

 if (!eInt_s) return eInt_s.as_unexpected<std::pair<int, int>>();

 if (!eInt_t) return eInt_t.as_unexpected<std::pair<int, int>>();

 return std::make_pair(*eInt_s, *eInt_t);
}

void PrintFraction(const std::pair<int, int>& f)
{
  std::cout << f.first << "/" << f.second <<"\n";
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
    std::cout << e.what() << std::endl;
  }
  catch(...)
  {
  }
}  

