#include <iostream>

namespace detail
{

template <typename F_t>
class NoExceptScopeGuard
{
  F_t f;
  bool run;

public:
  NoExceptScopeGuard(F_t f_) : f(f_), run(true) {}

  ~NoExceptScopeGuard()
  {
    if (run)
    {
      f();
    }     
  }

  NoExceptScopeGuard(const NoExceptScopeGuard& s) = delete;
  NoExceptScopeGuard(NoExceptScopeGuard&& s) : f(std::move(s.f)), run(s.run) { s.run = false; }

  NoExceptScopeGuard& operator=(const NoExceptScopeGuard& s) = delete;

  NoExceptScopeGuard& operator=(NoExceptScopeGuard&& s)
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

  void Release() { run = false; }
};

template <typename F_t>
class ScopeGuard
{
  F_t f;
  bool run;

public:
  ScopeGuard(F_t f_) : f(f_), run(true) {}

  ~ScopeGuard()
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

  ScopeGuard(const ScopeGuard& s) = delete;
  ScopeGuard(ScopeGuard&& s) : f(std::move(s.f)), run(s.run) { s.run = false; }

  ScopeGuard& operator=(const ScopeGuard& s) = delete;

  ScopeGuard& operator=(ScopeGuard&& s)
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

  void Release() { run = false; }
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
using if_type_else_type_t = typename if_type_else_type<b,T,U>::type;

template <typename F_t, typename ScopeGuard_T = if_type_else_type_t<noexcept(std::declval<F_t&>()()),
                                         detail::NoExceptScopeGuard<F_t>, detail::ScopeGuard<F_t>>>
auto make_ScopeGuard(F_t f)
{
  return ScopeGuard_T(std::move(f));
}

void f() noexcept
{
  std::cout << "Can I use a function pointer as a template argument? : Yes "
            << std::endl;
}

int main(int argc, char* argv[])
{
  auto printHelloWorldOnExit = make_ScopeGuard([]
                                               {
                                                 std::cout << "Hello world"
                                                           << std::endl;
                                               });
  auto printGoodbyeWorldOnExit =
      make_ScopeGuard([]
                      {
                        std::cout << "Goodbye cruel world" << std::endl;
                      });
  auto functionPointerTest = make_ScopeGuard(&f);
  printGoodbyeWorldOnExit.Release();

  auto exceptionsAreContained =
      make_ScopeGuard([]
                      {
                        throw std::runtime_error("Exception from within guard");
                      });
}
