#include <iostream>

class IVisitor;

class IVisitable
{
public:
  virtual void Accept(IVisitor& visitor) const = 0;
};

class VisitableA;
class VisitableB;
class VisitableC;

class IVisitor
{
public:
  virtual void Visit(const VisitableA& a) = 0;
  virtual void Visit(const VisitableB& b) = 0;
  virtual void Visit(const VisitableC& c) = 0;
};

class VisitableA : public IVisitable
{
public:
  void Accept(IVisitor& visitor) const override
  {
    visitor.Visit(*this);
  }

  const char* Name() const
  {
    return "A";
  }
};

class VisitableB : public IVisitable
{
public:
  void Accept(IVisitor& visitor) const override
  {
    visitor.Visit(*this);
  }

  const char* Name() const
  {
    return "B";
  }
};

class VisitableC : public IVisitable
{
public:
  void Accept(IVisitor& visitor) const override
  {
    visitor.Visit(*this);
  }

  const char* Name() const
  {
    return "C";
  }
};

class BaseVisitor : protected IVisitor
{
protected:
  BaseVisitor()
  {
  }

  void Visit(const VisitableA& a) override
  {
    std::cout << "I saw nothing\n";
  }

  void Visit(const VisitableB& b) override
  {
    std::cout << "I saw nothing\n";
  }

  void Visit(const VisitableC& c) override
  {
    std::cout << "I saw nothing\n";
  }
};

////////

struct NameReporter
{
  template <typename T>
  void operator()(const T& t)
  {
    std::cout << "I saw " << t.Name() << "\n";
  }
};

////////

template <typename... VisitableTypes_ts>
class VTVisitor;

template <typename T, typename F, typename... VisitableTypes_ts>
class VTVisitor<T, F, VisitableTypes_ts...>
    : public VTVisitor<VisitableTypes_ts...>
{
protected:
  void Visit(const T& v) override
  {
    F f;
    f(v);
  }
};

template <typename T, typename F>
class VTVisitor<T, F> : public T, public BaseVisitor
{
protected:
  void Visit(const T& v) override
  {
    F f;
    f(v);
  }
};

////////

template <typename T1, typename F1, typename T2, typename F2>
class TemplatedVisitor : public VTVisitor<T1, F1, T2, F2>
{
public:
  TemplatedVisitor(IVisitable& v)
  {
    v.Accept(*this);
  }
};

int main(int argc, char* argv[])
{
  VisitableA a;
  VisitableB b;
  VisitableC c;

  TemplatedVisitor<VisitableA, NameReporter, VisitableB, NameReporter> p_1(a);
  TemplatedVisitor<VisitableA, NameReporter, VisitableB, NameReporter> p_2(b);
  TemplatedVisitor<VisitableA, NameReporter, VisitableB, NameReporter> p_3(c);
}
