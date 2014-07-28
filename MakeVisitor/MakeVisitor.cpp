// Joint work by Rob Mill & J B Coe

#include <iostream>
#include <functional>

class A;
class B;
class C;

class AbstractVisitor
{
public:
  virtual void Visit(A& a) = 0;
  virtual void Visit(B& b) = 0;
  virtual void Visit(C& c) = 0;

protected:
  ~AbstractVisitor() {}
};


class Visitable
{
public:
  virtual void Accept(AbstractVisitor& v) = 0;

protected:
  ~Visitable() {}
};


class A : public Visitable
{
public:
  void Accept(AbstractVisitor& v) override { v.Visit(*this); }
};


class B : public Visitable
{
public:
  void Accept(AbstractVisitor& v) override { v.Visit(*this); }
};


class C : public Visitable
{
public:
  void Accept(AbstractVisitor& v) override { v.Visit(*this); }
};


template <typename T, typename F, typename BaseInnerVisitor, typename ArgsT>
class ComposeVisitor
{
public:
  class InnerVisitor : public BaseInnerVisitor
  {
  public:
    using BaseInnerVisitor::Visit;

    InnerVisitor(ArgsT&& args) : BaseInnerVisitor(std::move(args.second)), m_f(std::move(args.first)) {}

    void Visit(T& t) final override { m_f(t); }

  private:
    F m_f;
  };

  ComposeVisitor(ArgsT&& args) : m_args(std::move(args)) {}

  template <typename Tadd, typename Fadd>
  ComposeVisitor<Tadd, Fadd, InnerVisitor, std::pair<Fadd, ArgsT>> 
  on(Fadd&& f) &&
  {
    return ComposeVisitor<Tadd, Fadd, InnerVisitor, std::pair<Fadd, ArgsT>>(
        std::make_pair(std::move(f), std::move(m_args)));
  }

  template<typename InnerVisitor_=InnerVisitor>
  typename std::enable_if<!std::is_abstract<InnerVisitor_>::value, InnerVisitor>::type
  end_visitor() && { return InnerVisitor(std::move(m_args)); }

  ArgsT m_args;
};


template <typename TVisitorBase>
class EmptyVisitor
{
public:
  class InnerVisitor : public TVisitorBase
  {
  public:
    using TVisitorBase::Visit;

    InnerVisitor(std::nullptr_t) {}
  };

  template <typename Tadd, typename Fadd>
  ComposeVisitor<Tadd, Fadd, InnerVisitor, std::pair<Fadd, std::nullptr_t>> 
  on(Fadd&& f) &&
  {
    return ComposeVisitor<Tadd, Fadd, InnerVisitor, std::pair<Fadd, std::nullptr_t>>(
        std::make_pair(std::move(f), nullptr));
  }
};


template <typename TVisitorBase>
EmptyVisitor<TVisitorBase> begin_visitor()
{
  return EmptyVisitor<TVisitorBase>();
}


int main()
{
  A a;
  B b;
  C c;

  int iCounter = 0;

#ifdef USE_CLASS

  struct CVisitor : public AbstractVisitor
  {
    CVisitor(int& i) : iCounter_(i) {}

    int& iCounter_;

    void Visit(A& a) override final
    {
      std::cout << "I'm an A" << std::endl;
      iCounter_ += 1;
    }

    void Visit(B& a) override final
    {
      std::cout << "I'm a B" << std::endl;
      iCounter_ += 2;
    }
    
    void Visit(C& a) override final
    {
      std::cout << "I'm a C" << std::endl;
      iCounter_ += 3;
    }
  } visitor(iCounter);
#else
  auto visitor = begin_visitor<AbstractVisitor>()
                     .on<A>([&iCounter](A& a)
                            {
                              std::cout << "I'm an A" << std::endl;
                              iCounter += 1;
                            })
                     .on<B>([&iCounter](B& a)
                            {
                              std::cout << "I'm a B" << std::endl;
                              iCounter += 2;
                            })
                     .on<C>([&iCounter](C& c)
                            {
                              std::cout << "I'm a C" << std::endl;
                              iCounter += 3;
                           })
                     .end_visitor();

#endif                     

  a.Accept(visitor);
  b.Accept(visitor);
  c.Accept(visitor);

  std::cout << "Count is " << iCounter << std::endl;

  return 0;
}
