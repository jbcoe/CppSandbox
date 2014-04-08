#include <iostream>

template <typename T>
class NonOwningPointer // Minimal implementation of n3840's observer_pointer
{
public:

	  NonOwningPointer(T* p_) : p(p_) {}

    T* operator-> ()
    {
	      return p;
		}

		const T* operator-> () const
		{
			  return p;
		}

private:

		T* p;
};


struct Data
{
		void mutateMe() { x = true; }

		bool x = false;
};


struct DataObserver
{
	DataObserver(Data& d_) : d(&d_) {}

	void ThreadSafeConstMethod() const { d->mutateMe(); }

	NonOwningPointer<Data> d;
};


int main()
{
	Data d;
	const DataObserver o(d);
	
	std::cout << std::boolalpha << d.x << std::endl;
	
	o.ThreadSafeConstMethod();
	
	std::cout << std::boolalpha << d.x << std::endl;
}
