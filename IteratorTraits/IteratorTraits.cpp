#include <array>
#include <iostream>
#include <iterator>
#include <list>
#include <type_traits>
#include <vector>

namespace mpl
{
  template <class...>
  using void_ = void;

  template <class T>
  using diff_expr_t = decltype(std::declval<T>() - std::declval<T>());

  template <class T, class = void>
  struct difference_type
  {
    using type = long;
  };

  template <class T>
  struct difference_type<T, void_<diff_expr_t<T>>>
  {
    using type = diff_expr_t<T>;
  };

  template <typename T>
  using difference_type_t = typename difference_type<T>::type;
} // namespace mpl

template <typename T, typename iterator_category_t = std::input_iterator_tag>
struct deduced_iterator_traits
{
  using reference = decltype(*std::declval<T>());
  using pointer = std::add_pointer_t<std::remove_reference_t<reference>>;
  // value_type may not have the same cv-qualifers as
  // std::iterator_traits::value_type
  using value_type = std::remove_reference_t<reference>;
  using difference_type = mpl::difference_type_t<T>;
  using iterator_category = iterator_category_t;
};


#define static_iterator_traits_check(T)                                        \
  static_assert(                                                               \
      std::is_same<                                                            \
          std::remove_cv_t<std::iterator_traits<T>::value_type>,               \
          std::remove_cv_t<deduced_iterator_traits<T>::value_type>>::value,    \
      "");                                                                     \
  static_assert(std::is_same<std::iterator_traits<T>::pointer,                 \
                             deduced_iterator_traits<T>::pointer>::value,      \
                "");                                                           \
  static_assert(std::is_same<std::iterator_traits<T>::reference,               \
                             deduced_iterator_traits<T>::reference>::value,    \
                "");                                                           \
  static_assert(                                                               \
      std::is_same<std::iterator_traits<T>::difference_type,                   \
                   deduced_iterator_traits<T>::difference_type>::value,        \
      "")

#define COMMA ,

int main(int argc, char* argv[])
{
  static_iterator_traits_check(int*);
  static_iterator_traits_check(const int*);

  static_iterator_traits_check(std::vector<int>::iterator);
  static_iterator_traits_check(std::vector<const int>::iterator);

  static_iterator_traits_check(std::vector<int*>::iterator);
  static_iterator_traits_check(std::vector<const int*>::iterator);

  static_iterator_traits_check(std::list<int>::iterator);
  static_iterator_traits_check(std::list<const int>::iterator);

  static_iterator_traits_check(std::array<int COMMA 10>::iterator);
  static_iterator_traits_check(std::array<const int COMMA 10>::iterator);
}

