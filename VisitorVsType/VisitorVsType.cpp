#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <Common/Timer.h>

enum Type
{
  Type_A,
  Type_B,
};

struct Visitor;

struct O
{
  virtual void Accept(Visitor& v) = 0;
  virtual Type GetType() = 0;
};

struct A;
struct B;

struct Visitor
{
  virtual void Visit(A& o) = 0;
  virtual void Visit(B& o) = 0;
};

struct A : O
{
  virtual void Accept(Visitor& v)
  {
    v.Visit(*this);
  }
  Type GetType()
  {
    return Type_A;
  }
};

struct B : O
{
  virtual void Accept(Visitor& v)
  {
    v.Visit(*this);
  }
  Type GetType()
  {
    return Type_B;
  }
};

struct IsAGetter : Visitor
{
  IsAGetter() : m_bIsA(false)
  {
  }

  bool IsA(O& o)
  {
    o.Accept(*this);
    return m_bIsA;
  }

  void Visit(A& a)
  {
    m_bIsA = true;
  }
  void Visit(B& a)
  {
    m_bIsA = false;
  }

  bool m_bIsA;
};

int main(int argc, char** argv)
{
  int count = 100000000;
  if (argc > 1)
    count = atoi(argv[1]);

  int aCount = 0;
  int bCount = 0;
  std::vector<std::unique_ptr<O>> myObjects;

  bool bBuildA = true;
  std::generate_n(std::back_inserter(myObjects),
                  count, [&bBuildA]()->std::unique_ptr<O>
  {
    std::unique_ptr<O> ptr;
    if (bBuildA)
      ptr.reset(new A());
    else
      ptr.reset(new B());
    bBuildA = !bBuildA;
    return std::move(ptr);
  });

  int a_count0 = std::count_if(myObjects.begin(), myObjects.end(),
                               [&](std::unique_ptr<O> & po)
  { return po == 0; });

  int a_count1 = 0;
  {
    Timer t("Visitor");
    IsAGetter isAGetter;
    a_count1 = std::count_if(myObjects.begin(), myObjects.end(),
                             [&](std::unique_ptr<O> & po)
    { return isAGetter.IsA(*po); });
  }

  int a_count2 = 0;
  {
    Timer t("GetType");
    IsAGetter isAGetter;
    a_count2 = std::count_if(myObjects.begin(), myObjects.end(),
                             [&](std::unique_ptr<O> & po)
    { return po->GetType() == Type_A; });
  }


  return (a_count0 == count) && (a_count1 == a_count2);
}
