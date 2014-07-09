#include <iostream>

class VisitorBase;

class VisitableBase
{
public:
  virtual void Accept(VisitorBase& v) = 0;

  virtual ~VisitableBase(){}
};

class Visitable_A;
class Visitable_B;

class VisitorBase
{                
  public:
    virtual void Visit(Visitable_A& v) = 0;
    virtual void Visit(Visitable_B& v) = 0;
  
    virtual ~VisitorBase(){}
};

int main(int argc, char* argv[])
{
}

