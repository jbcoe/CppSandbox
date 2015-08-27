#include <iostream>
#include <random>
#include <functional>
#include "Common/inline_variant_visitor.h"
#include "Common/Timer.h"
#include <eggs/variant.hpp>

struct Increment
{
  double x_ = 0.0;
};

struct Multiplier
{
  double x_ = 0.0;
};

struct RelativeShift
{
  double x_ = 0.0;
};

struct Shifter
{
  double base_value_ = 0;
  Shifter(double x) : base_value_(x) {}
  double operator()(const Increment& i){ return base_value_ + i.x_; }
  double operator()(const Multiplier& i){ return base_value_ * i.x_; }
  double operator()(const RelativeShift& i){ return base_value_ * (1.0 + i.x_); }
};

int main(int argc, char* argv[])
{
  const size_t LOOPS = 100000000;

  std::uniform_int_distribution<int> distribution(0, 2);
  std::mt19937 engine(1001ul);
  auto shift_generator = std::bind(distribution, engine);
  
  {
    auto t = make_timer("Explicit visitor");
    double x = 1.0;
    eggs::variant<Increment,Multiplier,RelativeShift> v;
    for (size_t i=0;i<LOOPS;++i)
    {
      switch(shift_generator())
      {
        case 0: v = Increment();
                break;
        case 1: v = Multiplier();
                break;
        case 2: v = RelativeShift();
                break;
      }
      x = eggs::variants::apply<double>(Shifter(x), v);
    }
  }
  
  {
    auto t = make_timer("Inline visitor");
    double x = 1.0;
    eggs::variant<Increment,Multiplier,RelativeShift> v;
    for (size_t i=0;i<LOOPS;++i)
    {
      switch(shift_generator())
      {
        case 0: v = Increment();
                break;
        case 1: v = Multiplier();
                break;
        case 2: v = RelativeShift();
                break;
      }
      auto s = begin_variant_visitor()
        .on([x](const Increment& i){ return x+i.x_; })
        .on([x](const Multiplier& i){ return x * i.x_; })
        .on([x](const RelativeShift& i){ return x * (1.0 + i.x_); })
        .end_visitor();
        x = eggs::variants::apply<double>(s, v);
    }
  }
}

