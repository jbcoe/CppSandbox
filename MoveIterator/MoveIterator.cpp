#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
	vector<string> ingredients = {"slugs","snails","puppy-dogs' tails"};
	vector<string> ingredients2;
	copy(
			make_move_iterator(ingredients.begin()),
			make_move_iterator(ingredients.end()), 
			back_inserter(ingredients2));

  ostream_iterator<string> os(std::cout," ");
	
	std::copy(ingredients.begin(), ingredients.end(), os);
	std::cout << std::endl;
	
	std::copy(ingredients2.begin(), ingredients2.end(), os);
	std::cout << std::endl;
}

