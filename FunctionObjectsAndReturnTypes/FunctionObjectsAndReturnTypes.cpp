#include <iostream>
#include <functional>

int myAdd (int iFirst, int iSecond) 
{ 
	std::cout << iFirst << " " << iSecond << std::endl; 
	return iFirst + iSecond;
};

int main(int argc, char* argv[])
{
	std::function<int (int,int)> myF;

	myF = &myAdd;

	int a = myF(2,4);
	std::cout << "myF: " << myF(2,4) << std::endl;

	return 0;
}

