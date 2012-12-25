#include <iostream>
#include "Zoo.hpp"

int main(int argc, char* argv[])
{
	Zoo myZoo;
	for ( size_t i=0; i<myZoo.LionCount(); ++i )
	{
		std::cout << myZoo.GetLion(i)->makeNoise() << std::endl;
	}
}

