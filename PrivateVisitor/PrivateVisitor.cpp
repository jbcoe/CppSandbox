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
  virtual void Accept(IVisitor& visitor) const
  {
    visitor.Visit(*this);
  }
};

class VisitableB : public IVisitable
{
public:
  virtual void Accept(IVisitor& visitor) const
  {
    visitor.Visit(*this);
  }
};

class PrivateVisitor : private IVisitor
{
public:
  PrivateVisitor(IVisitable& v)
  {
    v.Accept(*this);
  }

private:
  virtual void Visit(const VisitableA& a)
  {
    std::cout << "I saw A\n";
  }

  virtual void Visit(const VisitableB& b)
  {
    std::cout << "I saw B\n";
  }
};

int main(int argc, char* argv[])
{
  VisitableA a;
  VisitableB b;

  PrivateVisitor p_a(a);
  PrivateVisitor p_b(b);
}
