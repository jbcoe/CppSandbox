#include <iostream>
#include <iterator>
#include <vector>

int main(int argc, char* argv[])
{
  std::vector<std::string> ingredients = {"slugs", "snails", "puppy-dogs' tails"};
  std::vector<std::string> ingredients2;
  std::copy(std::make_move_iterator(ingredients.begin()),
       std::make_move_iterator(ingredients.end()), std::back_inserter(ingredients2));

  std::ostream_iterator<std::string> os(std::cout, " ");

  std::copy(ingredients.begin(), ingredients.end(), os);
  std::cout << std::endl;

  std::copy(ingredients2.begin(), ingredients2.end(), os);
  std::cout << std::endl;
}
