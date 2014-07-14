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
    char c_;

    Data() : c_(0) {}
    ~Data() {}
  } data_;

  bool hasData_;

public:
  template <typename U>
  friend class Expected;

  Expected() : hasData_(false) {}

  template <typename E>
  Expected(Unexpected_T, E&& e)
      : hasData_(false)
  {
    try
    {
      throw e;
    }
    catch (...)
    {
      data_.e_ = std::current_exception();
    }
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

  explicit operator bool() const { return hasData_; }

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

  operator const T&() const
  {
    if (hasData_)
    {
      return data_.t_;
    }
    std::rethrow_exception(data_.e_);
  }

  operator T&()
  {
    if (hasData_)
    {
      return data_.t_;
    }
    std::rethrow_exception(data_.e_);
  }

  template <typename U>
  Expected<U> as_unexpected()
  {
    Expected<U> u;
    try
    {
      std::rethrow_exception(data_.e_);
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

int main(int argc, char* argv[])
{
  auto eInt = make_expected(5);
  std::cout << *eInt << std::endl;

  try
  {
    auto eError = make_unexpected<int>(std::runtime_error("Expected error"));
    auto eStrError = eError.as_unexpected<std::string>();
    *eStrError;
  }
  catch (const std::runtime_error& e)
  {
    std::cout << e.what() << std::endl;
  }
}

