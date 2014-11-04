#include <iostream>

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
      f();
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

template <typename F_t>
ScopeGuard<F_t> make_ScopeGuard(F_t f)
{
  return ScopeGuard<F_t>(f);
}

void f()
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
}
