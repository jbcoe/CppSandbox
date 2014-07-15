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


template <typename T, typename F, typename BaseInner, typename ArgsT>
class AddFunction
{
public:
  class Inner : public BaseInner
  {
  public:
    using BaseInner::Visit;

    Inner(ArgsT&& args) : BaseInner(std::move(args.second)), m_f(std::move(args.first)) {}

    void Visit(T& t) final override { m_f(t); }

  private:
    F m_f;
  };

  AddFunction(ArgsT&& args) : m_args(std::move(args)) {}

  template <typename Tadd, typename Fadd>
  AddFunction<Tadd, Fadd, Inner, std::pair<Fadd, ArgsT>> on(Fadd&& f) &&
  {
    return AddFunction<Tadd, Fadd, Inner, std::pair<Fadd, ArgsT>>(
        std::make_pair(std::move(f), std::move(m_args)));
  }

  Inner end_visitor() && { return Inner(std::move(m_args)); }

  ArgsT m_args;
};


template <typename TVisitorBase>
class EmptyVisitor
{
public:
  class Inner : public TVisitorBase
  {
  public:
    using TVisitorBase::Visit;

    Inner(std::nullptr_t) {}
  };

  template <typename Tadd, typename Fadd>
  AddFunction<Tadd, Fadd, Inner, std::pair<Fadd, std::nullptr_t>> on(Fadd&& f) &&
  {
    return AddFunction<Tadd, Fadd, Inner, std::pair<Fadd, std::nullptr_t>>(
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

  a.Accept(visitor);
  b.Accept(visitor);
  c.Accept(visitor);

  std::cout << "Count is " << iCounter << std::endl;

  return 0;
}
