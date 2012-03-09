#include <vector>
#include <iostream>                     
#include <algorithm>                     


class Copiable
{
public:
	
  static int iCopyCount;

	Copiable(int i) : m_i(i) {}
	
	Copiable(const Copiable& t) : m_i(t.m_i) 
	{
		if ( iCopyCount != -1 )
			++iCopyCount;
	}

  void operator = (const Copiable& t ) 
	{
		m_i = t.m_i;
		if ( iCopyCount != -1 )
			++iCopyCount;
	}

	operator int() { return m_i; }

private:
	
	int m_i;

};

int Copiable::iCopyCount = -1;

int main()
{
	std::vector<Copiable> myValues;
  for ( int i=0; i<100; ++i )
		myValues.push_back(i%3);

	std::cout << &myValues[0] << std::endl;
	
	int iOriginalSize = myValues.size();

	Copiable::iCopyCount = 0;

	myValues.erase(std::remove_if(myValues.begin(),myValues.end(),[](int i) { return i == 2; }),myValues.end());

	std::cout << &myValues[0] << std::endl;

	std::cout << "Removing " << iOriginalSize - myValues.size() 
		<< " values out of " << myValues.size() 
		<< " took " << Copiable::iCopyCount << " copies" << std::endl;

}
