#include <iostream>
#include <experimental/optional>

using std::experimental::nullopt;
using std::experimental::optional;

class Interface
{
public:
  virtual ~Interface()
  {
  }

  optional<double> Evaluate() const noexcept
  {
    try
    {
      return doEvaluate();
    }
    catch (...)
    {
      return nullopt;
    }
  }

private:
  virtual double doEvaluate() const = 0;
};

class Implementation : public Interface
{
public:
  Implementation(double a) : a_(a)
  {
  }

private:
  double a_ = 0.0;

  double doEvaluate() const override
  {
    if (a_ <= 0)
    {
      throw std::runtime_error("bad value for a");
    }
    return 2 * a_;
  }
};

int main(int argc, char* argv[])
{
  auto o = Implementation(0).Evaluate();
  if (o)
  {
    std::cout << *o << '\n';
  }
  o = Implementation(1).Evaluate();
  if (o)
  {
    std::cout << *o << '\n';
  }
  o = Implementation(2).Evaluate();
  if (o)
  {
    std::cout << *o << '\n';
  }
}
