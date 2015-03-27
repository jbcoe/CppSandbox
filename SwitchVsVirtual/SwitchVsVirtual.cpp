#include <iostream>
#include <random>
#include <array>
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
  
  Incrementer incrementer;
  Decrementer decrementer;
  std::array<Stepper*,2> steppers;
  steppers[0] = &incrementer;
  steppers[1] = &decrementer;
  
  int x=0;
  engine.seed(0);
  {
    Timer t("Virtual dispatch");
    for (size_t i=0;i<COUNT;++i)
    {
      x = steppers[generator()]->Step(x);
    }
  }

  NonVirtualStepper nvIncrementer(EStep::Increment);
  NonVirtualStepper nvDecrementer(EStep::Decrement);
  std::array<NonVirtualStepper*,2> nv_steppers;
  nv_steppers[0] = &nvIncrementer;
  nv_steppers[1] = &nvDecrementer;
  
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

