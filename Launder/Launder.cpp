struct A { virtual void f(); };
struct B : A { virtual void f(); };

void do_stuff() {
  B b;
  A *p = &b;
  p->f();
  p->f();
}

void do_stuff_2() {
  B b;
  A *p = &b;
  p->f();
  __builtin_launder(p)->f();
}


// elsewhere

void *operator new(decltype(sizeof(0)), void *p) { return p; }

struct C : A { virtual void f(); };

[[gnu::noinline]] void B::f() { /*...*/ new (this) C; }
[[gnu::noinline]] void C::f() { /*...*/ new (this) B; }
