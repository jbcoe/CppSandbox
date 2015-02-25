#include <iostream>
#include <functional>

#define NUMERIC_CALLBACKS_ENABLED 1

#ifdef NUMERIC_CALLBACKS_ENABLED
std::function<void(double original, double final)> f_onAssign;
std::function<void(double first, double second, double result)> f_onAdd;
std::function<void(double first, double second, double result)> f_onSubtract;
std::function<void(double first, double second, double result)> f_onMultiply;
std::function<void(double first, double second, double result)> f_onDivide;
#endif

template <typename Object_T>
class Numeric
{
public:
  typedef Numeric<Object_T> Numeric_T;

  Numeric(double value) : value_{value}
  {
  }

  Numeric_T operator+(const Numeric_T& that) const
  {
#ifdef NUMERIC_CALLBACKS_ENABLED
    if (f_onAdd)
    {
      f_onAdd(value_, that.value_, value_ + that.value_);
    }
#endif
    return Numeric_T(value_ + that.value_);
  }

  Numeric_T operator-(const Numeric_T& that) const
  {
#ifdef NUMERIC_CALLBACKS_ENABLED
    if (f_onSubtract)
    {
      f_onSubtract(value_, that.value_, value_ - that.value_);
    }
#endif
    return Numeric_T(value_ - that.value_);
  }

  Numeric_T operator*(const Numeric_T& that) const
  {
#ifdef NUMERIC_CALLBACKS_ENABLED
    if (f_onMultiply)
    {
      f_onMultiply(value_, that.value_, value_ * that.value_);
    }
#endif
    return Numeric_T(value_ * that.value_);
  }

  Numeric_T operator/(const Numeric_T& that) const
  {
#ifdef NUMERIC_CALLBACKS_ENABLED
    if (f_onDivide)
    {
      f_onDivide(value_, that.value_, value_ / that.value_);
    }
#endif
    return Numeric_T(value_ / that.value_);
  }

  double raw_value() const
  {
    return value_;
  }

private:
  double value_;
};

template <typename Object_T, typename OS_T>
OS_T& operator<<(OS_T& os, const Numeric<Object_T>& n)
{
  return os << n.raw_value();
}


class MyClass
{
};

typedef Numeric<MyClass> MyNumber;

MyNumber operator""_n(long double d)
{
  return MyNumber(static_cast<double>(d));
}

int main(int argc, char* argv[])
{

#ifdef NUMERIC_CALLBACKS_ENABLED
  f_onAdd = [](double a, double b, double c)
  {
    std::cout << a << " + " << b << " = " << c << std::endl;
  };
  f_onSubtract = [](double a, double b, double c)
  {
    std::cout << a << " - " << b << " = " << c << std::endl;
  };
  f_onMultiply = [](double a, double b, double c)
  {
    std::cout << a << " * " << b << " = " << c << std::endl;
  };
  f_onDivide = [](double a, double b, double c)
  {
    std::cout << a << " / " << b << " = " << c << std::endl;
  };
#endif

  auto result = 0.2_n + 0.3_n * 0.3_n - 0.8_n / 2.0_n;
  std::cout << "result = " << result << std::endl;
}
