#include <iostream>
#include <eggs/variant.hpp>

template <typename ...Ts>
using variant = eggs::variant<Ts...>;

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
      eggs::variants::apply<void>(*this, u);
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
  struct Inner
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

class A{};
class B{};
class C{};

struct X{ X(int){} };
struct Y{ Y(int){} };
struct Z{ Z(int){} };

int main(int argc, char* argv[])
{
  eggs::variant<A,B,C> v;
  eggs::variant<X,Y,Z> w;

  struct printer2
  {
    void operator()(X&) const { cout << "X\n"; }
    void operator()(Y&) const { cout << "Y\n"; }
    void operator()(Z&) const { cout << "Z\n"; }
  };

  try{
  eggs::variants::apply<void>(printer2(), w);
  }
  catch(const eggs::variants::bad_variant_access&)
  {
    std::cout << "Caught bad_variant_access\n";
  }

  /////////////////////////////////////////////////////////////
  
  struct printer
  {
    void operator()(A&) const { cout << "A\n"; }
    void operator()(B&) const { cout << "B\n"; }
    void operator()(C&) const { cout << "C\n"; }
  };

  v = A();
  eggs::variants::apply<void>(printer(), v);
  
  v = B();
  eggs::variants::apply<void>(printer(), v);
  
  v = C();
  eggs::variants::apply<void>(printer(), v);

  cout << "\n";

  /////////////////////////////////////////////////////////////
  
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
}

