#include <iostream>
#include <random>
#include <array>
#include <vector>
#include <memory>
#include "Common/Timer.h"

struct Stepper
{
  virtual int Step(int i) const = 0;
};

struct Incrementer : Stepper
{
  int Step(int i) const override { return i+1; }
};

struct Decrementer : Stepper
{
  int Step(int i) const override { return i-1; }
};

enum class EStep
{
  Increment,
  Decrement
};

struct NonVirtualStepper
{
  NonVirtualStepper(EStep step) : step_(step) {}
  
  EStep step_ = EStep::Increment;
  int Step(int i)
  {
    switch(step_)
    {
      case EStep::Increment:
        return i+1;
      case EStep::Decrement:
        return i-1;  
      default:
        return i;
    }              
  }
};

int main(int argc, char* argv[])
{
  size_t COUNT = 1000000;
  if (argc>1)
  {
    COUNT = std::stoul(argv[1]);
  }

  std::uniform_int_distribution<int> distribution(0, 1);
  std::mt19937 engine;
  auto generator = [&]{ return distribution(engine); };
  
  std::array<std::unique_ptr<Stepper>,2> steppers;
  steppers[0] = std::make_unique<Incrementer>();
  steppers[1] = std::make_unique<Decrementer>();
  
  int x=0;
  engine.seed(0);
  {
    Timer t("Virtual dispatch");
    for (size_t i=0;i<COUNT;++i)
    {
      x = steppers[generator()]->Step(x);
    }
  }

  std::array<std::unique_ptr<NonVirtualStepper>,2> nv_steppers;
  nv_steppers[0] = std::make_unique<NonVirtualStepper>(EStep::Increment);
  nv_steppers[1] = std::make_unique<NonVirtualStepper>(EStep::Decrement);
  
  int y=0;
  engine.seed(0);
  {
    Timer t("Enum-switch     ");
    for (size_t i=0;i<COUNT;++i)
    {
      y = nv_steppers[generator()]->Step(y);
    }
  }

  return (x-y);
}

