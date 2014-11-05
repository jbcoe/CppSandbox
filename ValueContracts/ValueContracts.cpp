#include <iostream>
#include <cmath>


#ifdef NCONTRACTS

template <typename T, typename... Requirements>
using Require = T;

#else

struct ContractViolation : public std::exception
{
  std::string m_what;

  template<typename T>
  ContractViolation(const T&)
  {
    m_what = "Contract violation: ";
    m_what += T::Name();
  }

  const char* what() const noexcept { return m_what.c_str(); } 
};

template <typename T, typename... Requirements>
class Require
{
  const T* t_;

  template <typename Requirement, typename... Rs>
  struct Validate
  {
    static void Check(const T& t)
    {
      Requirement r;
      if ( ! r(t) ) throw ContractViolation(r);
      Validate<Rs...>::Check(t);
    }
  };

  template <typename Requirement>
  struct Validate<Requirement>
  {
    static void Check(const T& t)
    {
      Requirement r;
      if ( ! r(t) ) throw ContractViolation(r);
    }
  };

public:
  Require(const T& t) : t_(&t) { Validate<Requirements...>::Check(*t_); }

  operator const T&() const { return *t_; }
};

#endif


struct Odd
{
  static const char* Name() { return "Odd"; }
  
  bool operator()(double d)
  {
    return (int)d % 2 == 1;
  }
};

struct Positive
{
  static const char* Name() { return "Positive"; }
  
  bool operator()(double d)
  {
    return d >= 0.0;
  }
};

double SquareRoot(Require<double, Positive, Odd> d) { return sqrt(d); }

int main(int argc, char* argv[])
{
  try
  {
    SquareRoot(2.0);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return -1;
  }
}

