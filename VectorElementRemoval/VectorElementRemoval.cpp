#include <vector>
#include <iostream>                     
#include <algorithm>                     


class Copyable
{
public:
	
  static int iCopyCount;

	Copyable(int i) : m_i(i) {}
	
	Copyable(const Copyable& t) : m_i(t.m_i) 
	{
		if ( iCopyCount != -1 )
			++iCopyCount;
	}

  void operator = (const Copyable& t ) 
	{
		m_i = t.m_i;
		if ( iCopyCount != -1 )
			++iCopyCount;
	}

	operator int() { return m_i; }

private:
	
	int m_i;

};

int Copyable::iCopyCount = -1;

int main()
{
	std::vector<Copyable> myValues;
  for ( int i=0; i<100; ++i )
		myValues.push_back(i%3);

	std::cout << &myValues[0] << std::endl;
	
	int iOriginalSize = myValues.size();

	Copyable::iCopyCount = 0;

	myValues.erase(std::remove_if(myValues.begin(),myValues.end(),[](int i) { return i == 2; }),myValues.end());

	std::cout << &myValues[0] << std::endl;

	std::cout << "Removing " << iOriginalSize - myValues.size() 
		<< " values out of " << myValues.size() 
		<< " took " << Copyable::iCopyCount << " copies" << std::endl;

}
