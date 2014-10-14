#include <iostream>

class IVisitor;

class IVisitable
{
public:
  virtual void Accept(IVisitor& visitor) const = 0;
};

class VisitableA;
class VisitableB;

class IVisitor
{
public:
  virtual void Visit(const VisitableA& a) = 0;
  virtual void Visit(const VisitableB& b) = 0;
};

class VisitableA : public IVisitable
{
public:
  void Accept(IVisitor& visitor) const override { visitor.Visit(*this); }
};

class VisitableB : public IVisitable
{
public:
  void Accept(IVisitor& visitor) const override { visitor.Visit(*this); }
};

class PrivateVisitor : private IVisitor
{
public:
  PrivateVisitor(IVisitable& v) { v.Accept(*this); }

private:
  void Visit(const VisitableA& a) override { std::cout << "I saw A\n"; }

  void Visit(const VisitableB& b) override { std::cout << "I saw B\n"; }
};

int main(int argc, char* argv[])
{
  VisitableA a;
  VisitableB b;

  PrivateVisitor p_a(a);
  PrivateVisitor p_b(b);
}
