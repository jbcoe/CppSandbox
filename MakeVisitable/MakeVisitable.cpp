//
// DO NOT USE THIS CODE
//
// This approach subverts a big advantage of visitors (forcing explicit consideration of types)
//

#include <iostream>

struct VisitorBase;

struct VisitableBase
{
  virtual void Accept(VisitorBase& v) = 0;

  virtual ~VisitableBase(){}
};

struct Visitable_A;
struct Visitable_B;

struct VisitorBase
{                
    virtual void Visit(const Visitable_A& v) = 0;
    virtual void Visit(const Visitable_B& v) = 0;
  
    virtual ~VisitorBase(){}
};

struct Visitable_A : public VisitableBase
{
  void Accept(VisitorBase& v) override { v.Visit(*this); }
};

struct Visitable_B : public VisitableBase
{                                       
  void Accept(VisitorBase& v) override { v.Visit(*this); }
};

struct NullVisitor : public VisitorBase
{
    void Visit(const Visitable_A& v) override {}
    void Visit(const Visitable_B& v) override {}
};

template <typename HandledType_T, typename HandlerFunction_T, typename Base_T>
struct ExtensibleVisitor : public Base_T
{
  ExtensibleVisitor(HandlerFunction_T f, Base_T& b) : Base_T(b), m_f(f) {}

  template <typename ExtraHandledType_T, typename ExtraHandlerFunction_T>
  auto on(ExtraHandlerFunction_T f)
  {
    return ExtensibleVisitor<
      ExtraHandledType_T, 
      ExtraHandlerFunction_T, 
      ExtensibleVisitor<HandledType_T,HandlerFunction_T,Base_T>>(f, *this);
  }
  
  void Visit(const HandledType_T& v) override
  {
    m_f(v);
  }

  HandlerFunction_T m_f;
};

template <typename Base_T>
struct BaseExtensibleVisitor : public Base_T
{
  BaseExtensibleVisitor(Base_T& b) : Base_T(b) {}

  template <typename ExtraHandledType_T, typename ExtraHandlerFunction_T>
  auto on(ExtraHandlerFunction_T f)
  {
    return ExtensibleVisitor<
      ExtraHandledType_T, 
      ExtraHandlerFunction_T, 
      BaseExtensibleVisitor<Base_T>>(f, *this);
  }
};

template<typename Base_T>
auto make_visitor(Base_T&& b)
{
  return BaseExtensibleVisitor<Base_T>(b);
}

int main(int argc, char* argv[])
{
  auto v = make_visitor(NullVisitor())
      .on<Visitable_A>([](auto& x){std::cout << "I saw an A ma!" << std::endl;})
      .on<Visitable_B>([](auto& x){std::cout << "I saw a B ma!" << std::endl;})
  ;

  Visitable_A().Accept(v);
  Visitable_B().Accept(v);
}

