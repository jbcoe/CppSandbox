#include <iostream>
#include <eggs/variant.hpp>

using std::nullptr_t;
using std::pair;
using std::make_pair;
using std::move;
using std::cout;


template <typename F, typename T>
concept bool Invokeable()
{
  return requires(const F& f, const T& t)
  {
    { f(t) } -> void;
  };
}

template <typename F, typename BaseInner, typename ArgsT>
struct ComposeVariantVisitor
{
  struct Inner : BaseInner
  {
    Inner(ArgsT&& a) : BaseInner(move(a.second)), f_(move(a.first)) {}

    using BaseInner::operator();

    template <typename T>
    requires Invokeable<F,T>()
    void operator()(const T& t) const
    {
      f_(t);
    }

  private:
    
    F f_;
  };

  ComposeVariantVisitor(ArgsT&& args) : m_args(move(args))
  {
  }

  template<typename Fadd>
  auto add_handler(Fadd&& f)
  {
    return ComposeVariantVisitor<Fadd, Inner, pair<Fadd, ArgsT>>(
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

  template <typename Fadd>
  auto add_handler(Fadd&& f)
  {
    return ComposeVariantVisitor<Fadd, Inner, pair<Fadd, nullptr_t>>(
        make_pair(move(f), nullptr));
  }
};

EmptyVariantVisitor begin_variant_visitor()
{
    return EmptyVariantVisitor();
}

class A{};
struct B{ void foo() const{} };
class C{};

template<typename T>
concept bool A_like()
{
  return std::is_same<A,T>::value;
}

template<typename T>
concept bool Fooer()
{
  return requires (const T& t)
  {
    { t.foo() } -> void
  };
}


int main(int argc, char* argv[])
{
  auto inline_printer = begin_variant_visitor()
    .add_handler([](const A_like&) { cout << "A-like\n";})
    .add_handler([](const Fooer&) { cout << "A fooer of some sort\n";}) 
    .add_handler([](const C&) { cout << "C\n";})
    .end_visitor();

  eggs::variant<A,B,C> v = B();
  eggs::variants::apply<void>(inline_printer, v);
}

