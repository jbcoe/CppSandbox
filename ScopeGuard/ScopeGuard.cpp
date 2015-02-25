#include <iostream>

namespace detail
{
  template <typename F_t>
  class no_except_scoped_guard
  {
    F_t f;
    bool run;

  public:
    no_except_scoped_guard(F_t f_) : f(f_), run(true)
    {
    }

    ~no_except_scoped_guard()
    {
      if (run)
      {
        f();
      }
    }

    no_except_scoped_guard(const no_except_scoped_guard& s) = delete;
    no_except_scoped_guard(no_except_scoped_guard&& s)
        : f(std::move(s.f)), run(s.run)
    {
      s.run = false;
    }

    no_except_scoped_guard& operator=(const no_except_scoped_guard& s) = delete;

    no_except_scoped_guard& operator=(no_except_scoped_guard&& s)
    {
      if (&s == this)
      {
        return *this;
      }

      f = std::move(s.f);
      run = s.run;
      s.run = false;
      return *this;
    }

    void release()
    {
      run = false;
    }
  };

  template <typename F_t>
  class scoped_guard
  {
    F_t f;
    bool run;

  public:
    scoped_guard(F_t f_) : f(f_), run(true)
    {
    }

    ~scoped_guard()
    {
      if (run)
      {
        try
        {
          f();
        }
        catch (...)
        {
        }
      }
    }

    scoped_guard(const scoped_guard& s) = delete;
    scoped_guard(scoped_guard&& s) : f(std::move(s.f)), run(s.run)
    {
      s.run = false;
    }

    scoped_guard& operator=(const scoped_guard& s) = delete;

    scoped_guard& operator=(scoped_guard&& s)
    {
      if (&s == this)
      {
        return *this;
      }

      f = std::move(s.f);
      run = s.run;
      s.run = false;
      return *this;
    }

    void release()
    {
      run = false;
    }
  };

} // end namespace detail

template <bool b, typename T, typename U>
struct if_type_else_type
{
};

template <typename T, typename U>
struct if_type_else_type<true, T, U>
{
  typedef T type;
};

template <typename T, typename U>
struct if_type_else_type<false, T, U>
{
  typedef U type;
};

template <bool b, typename T, typename U>
using if_type_else_type_t = typename if_type_else_type<b, T, U>::type;

template <typename F_t,
          typename scoped_guard_t = if_type_else_type_t<
              noexcept(std::declval<F_t&>()()),
              detail::no_except_scoped_guard<F_t>, detail::scoped_guard<F_t>>>
auto make_scoped_guard(F_t f)
{
  return scoped_guard_t(std::move(f));
}

void f() noexcept
{
  std::cout << "Can I use a function pointer as a template argument? : Yes "
            << std::endl;
}

int main(int argc, char* argv[])
{
  auto printHelloWorldOnExit = make_scoped_guard([]
                                                 {
                                                   std::cout << "Hello world"
                                                             << std::endl;
                                                 });
  auto printGoodbyeWorldOnExit =
      make_scoped_guard([]
                        {
                          std::cout << "Goodbye cruel world" << std::endl;
                        });

  auto functionPointerTest = make_scoped_guard(&f);
  printGoodbyeWorldOnExit.release();

  auto exceptionsAreContained = make_scoped_guard(
      []
      {
        throw std::runtime_error("Exception from within guard");
      });
}
