#define CATCH_CONFIG_MAIN
#include <cassert>
#include <catch.hpp>
#include <iostream>

template <typename T>
class optional
{
  template <typename U>
  struct is_optional : std::false_type
  {
  };
  template <typename U>
  struct is_optional<optional<U>> : std::true_type
  {
  };

  union data_t {
    char c_;
    T t_;

    data_t() : c_()
    {
    }

    ~data_t()
    {
    }
  };

  bool hasData_ = false;
  data_t data_;

public:
  void reset()
  {
    if (hasData_)
    {
      data_.t_.~T();
    }
    hasData_ = false;
  }

  ~optional()
  {
    reset();
  }

  optional() = default;

  template <typename U,
            typename = std::enable_if_t<!is_optional<U>::value &&
                                        std::is_constructible<T, U>::value>>
  explicit optional(U&& u)
  {
    ::new (std::addressof(data_.t_)) T(std::forward<U>(u));
    hasData_ = true;
  }

  template <typename U,
            typename = std::enable_if<std::is_constructible<T, U>::value>>
  optional(const optional<U>& o)
  {
    if (o.hasData_)
    {
      ::new (std::addressof(data_.t_)) T(o.data_.t_);
      hasData_ = true;
    }
  }

  template <typename U,
            typename = std::enable_if<std::is_constructible<T, U>::value>>
  optional(optional<U>&& o)
  {
    if (o.hasData_)
    {
      ::new (std::addressof(data_.t_)) T(std::move(o.data_.t_));
      hasData_ = true;
    }
  }

  template <typename U,
            typename = std::enable_if_t<!is_optional<U>::value &&
                                        std::is_constructible<T, U>::value>>
  optional& operator=(U&& u)
  {
    if (hasData_) reset();
    ::new (std::addressof(data_.t_)) T(std::forward<U>(u));
    hasData_ = true;
    return *this;
  }

  template <typename U,
            typename = std::enable_if_t<std::is_constructible<T, U>::value>>
  optional& operator=(const optional<U>& o)
  {
    if (hasData_) reset();

    if (o.hasData_)
    {
      ::new (std::addressof(data_.t_)) T(o.data_.t_);
      hasData_ = true;
    }
    return *this;
  }

  template <typename U,
            typename = std::enable_if_t<std::is_constructible<T, U>::value>>
  optional& operator=(const optional<U>&& o)
  {
    if (hasData_) reset();

    if (o.hasData_)
    {
      ::new (std::addressof(data_.t_)) T(std::move(o.data_.t_));
      hasData_ = true;
    }
    return *this;
  }

  explicit operator bool() const noexcept
  {
    return hasData_;
  }

  T& operator*()
  {
    assert(hasData_);
    return data_.t_;
  }

  const T& operator*() const
  {
    assert(hasData_);
    return data_.t_;
  }

  T* operator->()
  {
    assert(hasData_);
    return &data_.t_;
  }

  const T* operator->() const
  {
    assert(hasData_);
    return &data_.t_;
  }
};

template <typename T, typename U>
bool operator==(const optional<T>&, const optional<U>&) = delete;

template <typename T, typename U>
bool operator!=(const optional<T>&, const optional<U>&) = delete;

template <typename T, typename U>
bool operator<(const optional<T>&, const optional<U>&) = delete;

template <typename T, typename U>
bool operator>(const optional<T>&, const optional<U>&) = delete;

template <typename T, typename U>
bool operator<=(const optional<T>&, const optional<U>&) = delete;

template <typename T, typename U>
bool operator>=(const optional<T>&, const optional<U>&) = delete;


struct A
{
  int i_ = 0;

  A(int i) : i_(i) {}
};


SCENARIO("default construction", "[optional]")
{
  GIVEN("A default constructed optional")
  {
    optional<A> o;

    REQUIRE(!o);

    WHEN("reset is called")
    {
      o.reset();

      THEN("state is unchanged")
      {
        REQUIRE(!o);
      }
    }
    WHEN("a value is assigned to the optional")
    {
      o = A(5);

      THEN("the optional is engaged and the value is the assigned value")
      {
        REQUIRE(o);
        REQUIRE((*o).i_ == 5);
        REQUIRE(o->i_ == 5);
      }
    }
    WHEN("a convertible value is assigned to the optional")
    {
      o = 5;

      THEN("the optional is engaged and the value is the assigned value")
      {
        REQUIRE(o);
        REQUIRE((*o).i_ == 5);
        REQUIRE(o->i_ == 5);
      }
    }
  }
}

SCENARIO("value construction", "[optional]")
{
  GIVEN("A value constructed optional")
  {
    optional<A> o(A(9));

    REQUIRE(o);
    REQUIRE((*o).i_ == 9);
    REQUIRE(o->i_ == 9);

    WHEN("reset is called")
    {
      o.reset();

      THEN("state is disengaged")
      {
        REQUIRE(!o);
      }
    }
    WHEN("a value is assigned to the optional")
    {
      o = A(5);

      THEN("the optional is engaged and the value is the assigned value")
      {
        REQUIRE(o);
        REQUIRE((*o).i_ == 5);
        REQUIRE(o->i_ == 5);
      }
    }
    WHEN("a convertible value is assigned to the optional")
    {
      o = 5;

      THEN("the optional is engaged and the value is the assigned value")
      {
        REQUIRE(o);
        REQUIRE((*o).i_ == 5);
        REQUIRE(o->i_ == 5);
      }
    }
  }
}

SCENARIO("convertible-value construction", "[optional]")
{
  GIVEN("A convertible-value constructed optional")
  {
    optional<A> o(9);

    REQUIRE(o);
    REQUIRE((*o).i_ == 9);
    REQUIRE(o->i_ == 9);

    WHEN("reset is called")
    {
      o.reset();

      THEN("state is disengaged")
      {
        REQUIRE(!o);
      }
    }
    WHEN("a value is assigned to the optional")
    {
      o = A(5);

      THEN("the optional is engaged and the value is the assigned value")
      {
        REQUIRE(o);
        REQUIRE((*o).i_ == 5);
        REQUIRE(o->i_ == 5);
      }
    }
    WHEN("a convertible value is assigned to the optional")
    {
      o = 5;

      THEN("the optional is engaged and the value is the assigned value")
      {
        REQUIRE(o);
        REQUIRE((*o).i_ == 5);
        REQUIRE(o->i_ == 5);
      }
    }
  }
}
