#include <iostream>
#include "Zoo.hpp"

int main(int argc, char* argv[])
{
	Zoo myZoo;
	size_t lions = myZoo.LionCount();
	for ( size_t i=0; i<lions; ++i )
	{
		std::cout << myZoo.GetLion(i)->makeNoise() << std::endl;
	}
}

