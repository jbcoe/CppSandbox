#include <iostream>

template <typename Object_t, typename Underlying_t>
struct StrongTypedef
{
  StrongTypedef() : t_()
  {
  }

  explicit StrongTypedef(Underlying_t t) : t_(std::move(t))
  {
  }

  const auto& raw_data() const
  {
    return t_;
  }

  auto& raw_data()
  {
    return t_;
  }

private:
  Underlying_t t_;
};

#define STRICT_TYPES

#ifdef STRICT_TYPES
#define DEFINE_STRONG_TYPEDEF_OPERATOR(StrongTypedef_, operator_)              \
  StrongTypedef_ operator operator_(const StrongTypedef_& s1,                  \
                                    const StrongTypedef_& s2)                  \
  {                                                                            \
    return StrongTypedef_(s1.raw_data() operator_ s2.raw_data());              \
  }

#define DEFINE_STRONG_TYPEDEF_OPERATOR_RT(StrongTypedef_, RT, operator_)       \
  RT operator operator_(const StrongTypedef_& s1, const StrongTypedef_& s2)    \
  {                                                                            \
    return RT(s1.raw_data() operator_ s2.raw_data());                          \
  }

#define DEFINE_NUMERIC_TYPE(Type, Underlying)                                  \
  struct Type : StrongTypedef<Type, Underlying>                                \
  {                                                                            \
    using StrongTypedef<Type, Underlying>::StrongTypedef;                      \
  }
#else
#define DEFINE_NUMERIC_TYPE(Type, Underlying) typedef Underlying Type;
#define DEFINE_STRONG_TYPEDEF_OPERATOR(A,B)
#define DEFINE_STRONG_TYPEDEF_OPERATOR_RT(A,B,C)
#endif

DEFINE_NUMERIC_TYPE(Volatility, double);
DEFINE_STRONG_TYPEDEF_OPERATOR(Volatility, +);
DEFINE_STRONG_TYPEDEF_OPERATOR_RT(Volatility, double, *);

Volatility v1;

Volatility v2;

auto v3 = v1 + v2;

DEFINE_NUMERIC_TYPE(Length, double);
DEFINE_NUMERIC_TYPE(Area, double);
DEFINE_STRONG_TYPEDEF_OPERATOR_RT(Length, Area, *)
DEFINE_STRONG_TYPEDEF_OPERATOR(Length, +)


auto X = Length(1) * Length(3);

auto X2 = Length(1) + Length(3);

auto X3 = X + X2;

template <typename Object_t, typename Underlying_t>
class Numerical
{
public:
  typedef Numerical<Object_t, Underlying_t> Numerical_t;

  explicit Numerical(const Underlying_t& value) : m_value(value)
  {
  }

  Numerical() : m_value()
  {
  }
  
  Numerical(const Numerical_t& n) : m_value(n.m_value)
  {
  }
  
  Numerical_t& operator=(const Numerical_t& n)
  {
    m_value = n.m_value;
    return *this;
  }

  Numerical(Numerical_t&& n) : m_value(n.m_value)
  {
  }

  Numerical_t& operator=(Numerical_t&& n)
  {
    m_value = n.m_value;
    return *this;
  }

  Numerical_t& operator++()
  {
    ++m_value;
    return *this;
  }

  Underlying_t underlying_value() const
  {
    return m_value;
  }

private:
  Underlying_t m_value;
};

template <typename Object_t, typename Underlying_t>
bool operator!=(const Numerical<Object_t, Underlying_t>& n1,
                const Numerical<Object_t, Underlying_t>& n2)
{
  return n1.underlying_value() != n2.underlying_value();
}

template <typename Object_t, typename Underlying_t>
bool operator==(const Numerical<Object_t, Underlying_t>& n1,
                const Numerical<Object_t, Underlying_t>& n2)
{
  return n1.underlying_value() == n2.underlying_value();
}

template <typename Object_t, typename Underlying_t>
bool operator<(const Numerical<Object_t, Underlying_t>& n1,
               const Numerical<Object_t, Underlying_t>& n2)
{
  return n1.underlying_value() < n2.underlying_value();
}

template <typename Object_t, typename Underlying_t>
bool operator>(const Numerical<Object_t, Underlying_t>& n1,
               const Numerical<Object_t, Underlying_t>& n2)
{
  return n1.underlying_value() > n2.underlying_value();
}

template <typename Object_t, typename Underlying_t>
bool operator<=(const Numerical<Object_t, Underlying_t>& n1,
                const Numerical<Object_t, Underlying_t>& n2)
{
  return n1.underlying_value() <= n2.underlying_value();
}

template <typename Object_t, typename Underlying_t>
bool operator>=(const Numerical<Object_t, Underlying_t>& n1,
                const Numerical<Object_t, Underlying_t>& n2)
{
  return n1.underlying_value() >= n2.underlying_value();
}

template <typename Object_t, typename Underlying_t>
Numerical<Object_t, Underlying_t>
operator+(const Numerical<Object_t, Underlying_t>& n1,
          const Numerical<Object_t, Underlying_t>& n2)
{
  return Numerical<Object_t, Underlying_t>(n1.underlying_value() +
                                           n2.underlying_value());
}

template <typename Object_t, typename Underlying_t>
Numerical<Object_t, Underlying_t>
operator-(const Numerical<Object_t, Underlying_t>& n1,
          const Numerical<Object_t, Underlying_t>& n2)
{
  return Numerical<Object_t, Underlying_t>(n1.underlying_value() -
                                           n2.underlying_value());
}

template <typename OS_t, typename Object_t, typename Underlying_t>
OS_t& operator<<(OS_t& os, const Numerical<Object_t, Underlying_t>& n)
{
  return os << n.underlying_value();
}

template <typename Object_t>
using Int_t = Numerical<Object_t, int>;

template <typename Object_t>
using Double_t = Numerical<Object_t, double>;

template <typename Object_t>
using Index_t = Numerical<Object_t, size_t>;

#ifndef DEFINE_NUMERIC_TYPE
#define DEFINE_NUMERIC_TYPE(x, underlying_type)                                \
  class t_##x                                                                  \
  {                                                                            \
  };                                                                           \
  typedef Numerical<t_##x, underlying_type> x
#endif

DEFINE_NUMERIC_TYPE(Squirrel, int);
DEFINE_NUMERIC_TYPE(Speed, double);
DEFINE_NUMERIC_TYPE(Distance, double);
DEFINE_NUMERIC_TYPE(Time, double);

static Speed operator/(const Distance& d, const Time& t)
{
  return Speed(d.underlying_value() / t.underlying_value());
}

static Distance operator*(const Time& t, const Speed& s)
{
  return Distance(t.underlying_value() * s.underlying_value());
}

static Distance operator*(const Speed& s, const Time& t)
{
  return Distance(s.underlying_value() * t.underlying_value());
}

//////////////////////////////////////////////

int main(int argc, char* argv[])
{
  DEFINE_NUMERIC_TYPE(Apple, int);
  DEFINE_NUMERIC_TYPE(Orange, int);

  Apple myAppleCount(5);
  Apple yourAppleCount(8);

  auto ourAppleCount = myAppleCount + yourAppleCount;
  std::cout << "My Apples: " << myAppleCount << "\n";
  std::cout << "Your Apples: " << yourAppleCount << "\n";
  std::cout << "Our Apples: " << ourAppleCount << "\n";

  std::cout << "\n";

  Orange myOrangeCount(5);
  Orange yourOrangeCount(8);

  auto ourOrangeCount = myOrangeCount + yourOrangeCount;
  std::cout << "My Oranges: " << myOrangeCount << "\n";
  std::cout << "Your Oranges: " << yourOrangeCount << "\n";
  std::cout << "Our Oranges: " << ourOrangeCount << "\n";

  std::cout << "\n";

  for (Orange i(1), five_oranges(5); i <= five_oranges; ++i)
  {
    std::cout << "Orange counter: " << i << "\n";
  }

  // One cannot add apples to oranges
  // auto ourFruitCount = ourAppleCount + ourOrangeCount;

  auto s = Distance(10.0) / Time(8.0);

  std::cout << "\n";

  std::cout << "Speed " << s << " = Distance " << Distance(10.0) << " / Time "
            << Time(8.0) << "\n";
}
