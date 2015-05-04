#include <iostream>
#include <boost/variant.hpp>

using std::nullptr_t;
using std::pair;
using std::make_pair;
using std::move;

template <typename T, typename F, typename BaseInner, typename ArgsT>
struct ComposeVariantVisitor
{
  struct Inner : BaseInner
  {
    Inner(ArgsT&& a) : BaseInner(std::move(a.second)), f_(std::move(a.first)) {}

    using BaseInner::operator();

    void operator()(T& t) const { f_(t); }

private:
    
    F f_;
  };

  ComposeVariantVisitor(ArgsT&& args) : m_args(move(args))
  {
  }

  template<typename Tadd, typename Fadd>
  auto on(Fadd&& f)
  {
    return ComposeVariantVisitor<Tadd, Fadd, Inner, std::pair<Fadd, ArgsT>>(
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

class A{};
class B{};
class C{};

int main(int argc, char* argv[])
{
  boost::variant<A,B,C> v;

  /////////////////////////////////////////////////////////////
  
  struct printer : boost::static_visitor<>
  {
    void operator()(A&) const { std::cout << "A\n"; }
    void operator()(B&) const { std::cout << "B\n"; }
    void operator()(C&) const { std::cout << "C\n"; }
  };

  v = A();
  boost::apply_visitor(printer(), v);
  
  v = B();
  boost::apply_visitor(printer(), v);
  
  v = C();
  boost::apply_visitor(printer(), v);

  std::cout << "\n";

  /////////////////////////////////////////////////////////////
  
  auto inline_printer = begin_variant_visitor()
    .on<A>([](A&) { std::cout << "A\n";})
    .on<B>([](B&) { std::cout << "B\n";})
    .on<C>([](C&) { std::cout << "C\n";})
    .end_visitor();

  v = A();
  boost::apply_visitor(inline_printer, v);
  
  v = B();
  boost::apply_visitor(inline_printer, v);
  
  v = C();
  boost::apply_visitor(inline_printer, v);
}

