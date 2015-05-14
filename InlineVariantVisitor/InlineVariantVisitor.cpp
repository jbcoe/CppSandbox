#include <iostream>
#include <boost/variant.hpp>

using std::nullptr_t;
using std::pair;
using std::make_pair;
using std::move;
using std::cout;

template <typename T, typename F, typename BaseInner, typename ArgsT>
struct ComposeVariantVisitor
{
  struct Inner : BaseInner
  {
    Inner(ArgsT&& a) : BaseInner(move(a.second)), f_(move(a.first)) {}

    using BaseInner::operator();

    void operator()(T& t) const { f_(t); }

    template<typename U>
    void match(U& u)
    {
      boost::apply_visitor(*this, u);
    }

  private:
    
    F f_;
  };

  ComposeVariantVisitor(ArgsT&& args) : m_args(move(args))
  {
  }

  template<typename Tadd, typename Fadd>
  auto on(Fadd&& f)
  {
    return ComposeVariantVisitor<Tadd, Fadd, Inner, pair<Fadd, ArgsT>>(
        make_pair(move(f), move(m_args)));
  }
  
  auto end_visitor()
  {
    return Inner(move(m_args));
  }

  ArgsT m_args;
};

struct EmptyVariantVisitor
{
  struct Inner : public boost::static_visitor<>
  {                  
    struct detail_t {};

    Inner(nullptr_t)
    {
    }

    void operator()(detail_t&) const {}
  };

  template <typename Tadd, typename Fadd>
  auto on(Fadd&& f)
  {
    return ComposeVariantVisitor<Tadd, Fadd, Inner, pair<Fadd, nullptr_t>>(
        make_pair(move(f), nullptr));
  }
};

EmptyVariantVisitor begin_variant_visitor()
{
    return EmptyVariantVisitor();
}

struct A
{
  void Foo() const {}
  void Bar() const {}
};

struct B
{
  void Foo() const {}
};

struct C
{
  void Bar() const {}
};

struct D
{
};

template <typename T>
concept bool HasFoo()
{
  return requires(const T& t)
  {
    t.Foo();
  };
}

template <typename T>
concept bool HasBar()
{
  return requires(const T& t)
  {
    t.Bar();
  };
}

struct printer : boost::static_visitor<>
  {
    void operator()(A&) const { cout << "A\n"; }
    void operator()(HasBar&) const { cout << "Something with a Bar\n"; }
    void operator()(HasFoo&) const { cout << "Something with a Foo\n"; }
    void operator()(auto&) const { cout << "Something completely different\n"; }
  };

int main(int argc, char* argv[])
{
  boost::variant<A,B,C,D> v;

  /////////////////////////////////////////////////////////////
  

  v = A();
  boost::apply_visitor(printer(), v);
  
  v = B();
  boost::apply_visitor(printer(), v);
  
  v = C();
  boost::apply_visitor(printer(), v);

  v = D();
  boost::apply_visitor(printer(), v);
  
  cout << "\n";

  /////////////////////////////////////////////////////////////
  /*
  auto inline_printer = begin_variant_visitor()
    .on<A>([](A&) { cout << "A\n";})
    .on<B>([](B&) { cout << "B\n";})
    .on<C>([](C&) { cout << "C\n";})
    .end_visitor();

  v = A();
  inline_printer.match(v);
  
  v = B();
  inline_printer.match(v);
  
  v = C();
  inline_printer.match(v);
  */
}

