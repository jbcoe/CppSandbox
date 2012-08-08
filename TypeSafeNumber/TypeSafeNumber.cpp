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

		// If you want operator * implementing then add it yourself using inheritance

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

//////////////////////////////////////////////

class Distance;
class Time;
class Speed;

/////////////////////////////////////////////

class Distance : public Double_t<Distance> 
{
	public:

		explicit Distance (double v) : Double_t<Distance>(v) {}

		Speed operator / (const Time& t) const;
};

/////////////////////////////////////////////

class Time : public Double_t<Time> 
{
	public:

		explicit Time (double v) : Double_t<Time>(v) {}

		Distance operator * (const Speed& s) const;
};

/////////////////////////////////////////////

class Speed : public Double_t<Speed> 
{
	public:
		explicit Speed (double v) : Double_t<Speed>(v) {}

		Distance operator * (const Time& t) const;
};

//////////////////////////////////////////////

Speed Distance::operator / (const Time& t) const 
{ 
	return Speed(underlying_value()/t.underlying_value());
}

Distance Time::operator * (const Speed& s) const 
{ 
	return Distance(underlying_value()*s.underlying_value());
}

Distance Speed::operator * (const Time& t) const 
{ 
	return Distance(underlying_value()*t.underlying_value());
}

//////////////////////////////////////////////

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

	for ( Index_t<Oranges> i(1), five_oranges(5); i<=five_oranges; ++i )
	{
		std::cout << "Orange counter: " << i << "\n";
	}

	// One cannot add apples to oranges
	//auto ourFruitCount = ourAppleCount + ourOrangeCount; 
	
  Speed s = Distance(10.0) / Time(8.0);

	std::cout << "\n";
	
	std::cout << "Speed " << s << " = Distance " << Distance(10.0) << " / Time " << Time(8.0) << "\n"; 

}

