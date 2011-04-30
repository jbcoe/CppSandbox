#include <iostream>

template <int argumentCount, typename T, typename... Values>
int _countTemplateArguments()
{
	return _countTemplateArguments<argumentCount+1, Values>();
}

template <int argumentCount, typename T>
int _countTemplateArguments()
{
	return argumentCount + 1;
}

template <int argumentCount>
int _countTemplateArguments()
{
	return 0;
}

template <typename T, typename... Values>
int countTemplateArguments()
{
	return _countTemplateArguments<0, T, Values>();
}

int main(int argc, char* argv[])
{
	std::cout << countTemplateArguments<1,2,3,4,5>() << std::endl;
}

