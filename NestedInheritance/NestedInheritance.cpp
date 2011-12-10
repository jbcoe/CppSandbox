#include <iostream>

class Base 
{
	public:
	~Base(){}
	virtual	void DoSomething() const {}
	virtual	void DoSomethingElse() const {}
};

template <class TInterface>
class VirtualWrapper_A : public virtual TInterface { protected: ~VirtualWrapper_A(){} };

template <class TInterface>
class VirtualWrapper_B : public virtual TInterface { protected: ~VirtualWrapper_B(){} };

template <class TInterface>
class Wrapper_A : public TInterface { protected: ~Wrapper_A(){} };

template <class TInterface>
class Wrapper_B : public TInterface { protected: ~Wrapper_B(){} };

class VirtualDerived : public VirtualWrapper_A<Base>, public VirtualWrapper_B<Base> {};

class NestedDerived : public VirtualWrapper_A<VirtualWrapper_B<Base>> {};

int main(int argc, char* argv[])
{
	VirtualDerived vd;
	vd.DoSomething();
	
	NestedDerived nd;
	nd.DoSomething();

	std::cout << "sizeof(Base) " << sizeof(Base) << std::endl;
	std::cout << "sizeof(VirtualDerived) " << sizeof(vd) << std::endl;
	std::cout << "sizeof(NestedDerived) " << sizeof(nd) << std::endl;
}

