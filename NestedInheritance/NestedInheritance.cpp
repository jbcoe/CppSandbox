#include <iostream>

struct DataForBase { int m_base; };

struct DataForA { int m_a; };

struct DataForB { int m_b; };

class Interface 
{
	public:
		virtual ~Interface(){}
		virtual	void DoBase() const = 0 ;
		virtual	void DoA() const = 0;
		virtual	void DoB() const = 0;
};

template <class TInterface>
class VirtualWrapper_A : public virtual TInterface 
{ 
	private: 
		DataForA m_aData;

	protected: 
		~VirtualWrapper_A(){}  

	public:
		virtual void DoA() const {}
};

template <class TInterface>
class VirtualWrapper_B : public virtual TInterface 
{ 
	private:
		DataForB m_bData;

	protected: 
		~VirtualWrapper_B(){} 

	public:
		virtual void DoB() const {}
};

template <class TInterface>
class Wrapper_A : public TInterface 
{ 
	private: 
		DataForA m_aData;

	protected: 
		~Wrapper_A(){} 

	public:
		virtual void DoA() const {}
};

template <class TInterface>
class Wrapper_B : public TInterface 
{ 
	private:
		DataForB m_bData;

	protected: 
		~Wrapper_B(){} 

	public:
		virtual void DoB() const {}
};

class VirtualDerived : public VirtualWrapper_A<Interface>, public VirtualWrapper_B<Interface>
{
	private:
		DataForBase m_baseData;

	public:
		virtual void DoBase() const {};
};

class NestedDerived : public VirtualWrapper_A<VirtualWrapper_B<Interface>> 
{
	private:
		DataForBase m_baseData;

	public:
		virtual void DoBase() const {};
};

class DelegateA
{
	DataForA m_base;
	public:
	virtual void DoA() const {}
};

class DelegateB
{
	DataForB m_base;
	public:
	virtual void DoB() const {}
};

class DelegatingImpl : public Interface
{
	private:
		DelegateA m_dA;
		DelegateB m_dB;

	public:
		virtual void DoBase() const { }
		virtual void DoA() const { m_dA.DoA(); }
		virtual void DoB() const { m_dB.DoB(); }
};

class RawImpl : public Interface
{
	private:
		DataForBase m_baseData;
		DataForA m_aData;
		DataForB m_bData;
	
	public:
		virtual void DoBase() const {}
		virtual void DoA() const {}
		virtual void DoB() const {}
};

int main(int argc, char* argv[])
{
	VirtualDerived vd;
	vd.DoBase();
	vd.DoA();
	vd.DoB();

	NestedDerived nd;
	nd.DoBase();
	nd.DoA();
	nd.DoB();

	DelegatingImpl di;
	di.DoBase();
	di.DoA();
	di.DoB();

	RawImpl ri;
	ri.DoBase();
	ri.DoA();
	ri.DoB();

#define PRINT_SIZE_OF(thing) std::cout << "sizeof("#thing") " << sizeof(thing) << std::endl

  PRINT_SIZE_OF(void*);
  PRINT_SIZE_OF(int);
  PRINT_SIZE_OF(Interface);
  PRINT_SIZE_OF(DataForBase);
  PRINT_SIZE_OF(DataForA);
  PRINT_SIZE_OF(DataForB);
  PRINT_SIZE_OF(RawImpl);
  PRINT_SIZE_OF(VirtualDerived);
  PRINT_SIZE_OF(NestedDerived);
  PRINT_SIZE_OF(DelegatingImpl);

#undef PRINT_SIZE_OF

}

