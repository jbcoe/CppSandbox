#include <iostream>

int main(int argc, char* argv[])
{            
	int myAppleCount(5);
	int yourAppleCount(8);

	auto ourAppleCount = myAppleCount + yourAppleCount;
	std::cout << "My Apples: " << myAppleCount << "\n";
	std::cout << "Your Apples: " << yourAppleCount << "\n";
	std::cout << "Our Apples: " << ourAppleCount << "\n";

	std::cout << "\n";

	int myOrangeCount(5);
	int yourOrangeCount(8);

	auto ourOrangeCount = myOrangeCount + yourOrangeCount;
	std::cout << "My Oranges: " << myOrangeCount << "\n";
	std::cout << "Your Oranges: " << yourOrangeCount << "\n";
	std::cout << "Our Oranges: " << ourOrangeCount << "\n";

	std::cout << "\n";

	for ( size_t i(1), five_oranges(5); i<=five_oranges; ++i )
	{
		std::cout << "Orange counter: " << i << "\n";
	}

	// One cannot add apples to oranges
	//auto ourFruitCount = ourAppleCount + ourOrangeCount; 
	
  typedef double Speed;
  typedef double Distance;
  typedef double Time;

  Speed s = Distance(10.0) / Time(8.0);

	std::cout << "\n";
	
	std::cout << "Speed " << s << " = Distance " << Distance(10.0) << " / Time " << Time(8.0) << "\n"; 

}

