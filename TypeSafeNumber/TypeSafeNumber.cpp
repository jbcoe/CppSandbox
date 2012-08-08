#include <iostream>

template <typename Object_t, typename Underlying_t>
class Numerical
{
public:

	typedef Numerical<Object_t, Underlying_t> Numerical_t;

	explicit Numerical(const Underlying_t& value) : m_value(value) {}
	Numerical() : m_value() {}
  Numerical(const Numerical_t& n) : m_value(n.m_value) {}
  const Numerical& operator = (const Numerical_t& n) { m_value = n.m_value; return *this; }
  
	bool operator != (const Numerical_t& n) { return m_value != n.m_value; }
  bool operator == (const Numerical_t& n) { return m_value == n.m_value; }
  bool operator < (const Numerical_t& n) { return m_value < n.m_value; }
  bool operator > (const Numerical_t& n) { return m_value > n.m_value; }
  bool operator <= (const Numerical_t& n) { return m_value <= n.m_value; }
  bool operator >= (const Numerical_t& n) { return m_value >= n.m_value; }

	Numerical_t& operator ++ () { ++m_value; return *this; }
	
	Numerical_t operator + ( const Numerical_t& n) const { return Numerical_t(m_value + n.m_value); }
	Numerical_t operator - ( const Numerical_t& n) const { return Numerical_t(m_value - n.m_value); }
	Underlying_t operator / ( const Numerical_t& n) const { return m_value / n.m_value; }

	Underlying_t underlying_value() const { return m_value; }

private:
	Underlying_t m_value;
};

template <typename Object_t, typename Underlying_t>
std::ostream& operator << ( std::ostream& os, const Numerical<Object_t, Underlying_t>& n)
{
	return os << n.underlying_value();
}

template <typename Object_t>
using Int_t = Numerical<Object_t,int>;

template <typename Object_t>
using Double_t = Numerical<Object_t,double>;

template <typename Object_t>
using Index_t = Numerical<Object_t,size_t>;

class Apples {};
class Oranges {};

int main(int argc, char* argv[])
{            
	Int_t<Apples> myAppleCount(5);
	Int_t<Apples> yourAppleCount(8);
	
  auto ourAppleCount = myAppleCount + yourAppleCount;
	std::cout << "My Apples: " << myAppleCount << "\n";
	std::cout << "Your Apples: " << yourAppleCount << "\n";
	std::cout << "Our Apples: " << ourAppleCount << "\n";

	std::cout << "\n";

	Int_t<Oranges> myOrangeCount(5);
	Int_t<Oranges> yourOrangeCount(8);
	
  auto ourOrangeCount = myOrangeCount + yourOrangeCount;
	std::cout << "My Oranges: " << myOrangeCount << "\n";
	std::cout << "Your Oranges: " << yourOrangeCount << "\n";
	std::cout << "Our Oranges: " << ourOrangeCount << "\n";

	std::cout << "\n";
	
  for ( Index_t<Oranges> i(1), ten_oranges(10); i<=ten_oranges; ++i )
	{
		std::cout << "Orange counter: " << i << "\n";
	}

	// One cannot add apples to oranges
	//auto ourFruitCount = ourAppleCount + ourOrangeCount; 
}

