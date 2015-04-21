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

    void operator()(const T& t) const { f_(t); }

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
  
  auto end_variant_visitor()
  {
    return Inner(move(m_args));
  }

  ArgsT m_args;
};

struct EmptyVariantVisitor
{
  struct Inner : public boost::static_visitor<>
  {
    Inner(nullptr_t)
    {
    }
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


int main(int argc, char* argv[])
{
  boost::variant<int,double,char> v;

  struct printer : boost::static_visitor<>
  {
    void operator()(const int i) const { std::cout << i << "\n"; }
    void operator()(const double d) const { std::cout << d << "\n"; }
    void operator()(const char c) const { std::cout << c << "\n"; }
  };

  v = 7;
  boost::apply_visitor(printer(), v);
  
  v = 3.14159;
  boost::apply_visitor(printer(), v);
  
  v = 'p';
  boost::apply_visitor(printer(), v);

  auto inline_printer = begin_variant_visitor()
    .on<int>([](const int i) { std::cout << i << "\n";})
    .on<double>([](const double d) { std::cout << d << "\n";})
    .on<char>([](const char c) { std::cout << c << "\n";})
    .end_variant_visitor();

  v = 7;
  boost::apply_visitor(inline_printer, v);
  
  v = 3.14159;
  boost::apply_visitor(inline_printer, v);
  
  v = 'p';
  boost::apply_visitor(inline_printer, v);
}

