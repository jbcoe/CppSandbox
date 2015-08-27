#include <utility>

// These using declarations are for publication brevity only
using std::make_pair;
using std::move;
using std::pair;
using std::nullptr_t;

template <typename F, typename T> concept bool Invokeable() {
  return requires(const F &f, T &t) {
    { f(t) }
  };
}

template <typename F, typename BaseInner, typename ArgsT>
struct ComposeVariantVisitor {
  struct Inner : BaseInner {
    Inner(ArgsT &&a) : BaseInner(move(a.second)), f_(move(a.first)) {}

    using BaseInner::operator();

    template <typename T>
    requires Invokeable<F, T>() auto operator()(T &t) {
      return f_(t);
    }

  private:
    F f_;
  };

  ComposeVariantVisitor(ArgsT &&args) : m_args(move(args)) {}

  template <typename Fadd> auto on(Fadd &&f) {
    return ComposeVariantVisitor<Fadd, Inner, pair<Fadd, ArgsT>>(
        make_pair(move(f), move(m_args)));
  }

  auto end_visitor() { return Inner(move(m_args)); }

  ArgsT m_args;
};

struct EmptyVariantVisitor {
  struct Inner {
    struct detail_t {};

    Inner(nullptr_t) {}

    void operator()(detail_t &) const {}
  };

  template <typename Fadd> auto on(Fadd &&f) {
    return ComposeVariantVisitor<Fadd, Inner, pair<Fadd, nullptr_t>>(
        make_pair(move(f), nullptr));
  }
};

EmptyVariantVisitor begin_variant_visitor() { return EmptyVariantVisitor(); }

