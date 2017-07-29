#include <iostream>

template <class F, class... Ts>
void apply(F f, Ts&&... ts) {
  ((f(std::forward<Ts>(ts))), ...);
}

int main(int argc, char* argv[]) {
  apply([](const auto& x) { std::cout << x << '\n'; }, 1, 2, 3, "Hello");
}

