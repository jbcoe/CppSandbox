#include <iostream>

struct Base 
{
	void DoThing() final const 
	{
	}
};

struct Derived final : Base
{
};

struct FurtherDerived : Derived
{
};

int main(int argc, char* argv[])
{
}

