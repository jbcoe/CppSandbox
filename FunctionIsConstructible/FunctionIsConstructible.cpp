#include <functional>
#include <type_traits>
#include <iostream>

int main(int argc, char* argv [])
{
	auto f_1 = [](int x) { return x; };
	std::cout << "auto f_1 = [](int x) { return x; };\n";

	auto f_2 = [](int x, int y) { return x + y; };
	std::cout << "auto f_2 = [](int x, int y) { return x + y; };\n";

	std::cout << "\n";

	std::cout << "std::is_constructible<std::function<int(int)>, decltype(f_1)>::value "
		<< std::is_constructible<std::function<int(int)>, decltype(f_1)>::value << "\n";

	std::cout << "std::is_constructible<std::function<int(int)>, decltype(f_2)>::value "
		<< std::is_constructible<std::function<int(int)>, decltype(f_2)>::value << "\n";

	std::cout << "\n";

	std::cout << "std::is_constructible<std::function<int(int,int)>, decltype(f_1)>::value "
		<< std::is_constructible<std::function<int(int, int)>, decltype(f_1)>::value << "\n";

	std::cout << "std::is_constructible<std::function<int(int,int)>, decltype(f_2)>::value "
		<< std::is_constructible<std::function<int(int, int)>, decltype(f_2)>::value << "\n";


	std::function<int(int)> f_i_i(f_1);
	//std::function<int(int)> f_i_ii(f_2); // does not compile as it is not constructible

	//std::function<int(int,int)> f_ii_i(f_1); // does not compile as it is not constructible
	std::function<int(int,int)> f_ii_ii(f_2);

	return 0;
}
