#include <iostream>

class IVisitor;

class IVisitable
{
	public:
		virtual void Accept(IVisitor& visitor) const = 0;
};

class VisitableA;
class VisitableB;

class IVisitor
{
	public:
		virtual void Visit(const VisitableA& a) = 0;
		virtual void Visit(const VisitableB& b) = 0;
};

class VisitableA : public IVisitable
{
	public:
		virtual void Accept(IVisitor& visitor) const
		{
			visitor.Visit(*this);
		}
};

class VisitableB : public IVisitable
{
	public:
		virtual void Accept(IVisitor& visitor) const
		{
			visitor.Visit(*this);
		}
};

class BaseVisitor : protected IVisitor
{
	protected:
		BaseVisitor() {}

		virtual void Visit(const VisitableA& a)
		{
			std::cout << "I saw nothing\n";
		}

		virtual void Visit(const VisitableB& b)
		{
			std::cout << "I saw nothing\n";
		}
};

////////

template <typename... VisitableTypes_ts> class VTVisitor;

template <typename T, typename... VisitableTypes_ts>
class VTVisitor<T, VisitableTypes_ts...> : public VTVisitor<VisitableTypes_ts...>
{
	protected:
		virtual void Visit(const T& v)
		{
			std::cout << "I saw a type I knew\n";
		}
};

template <typename T>
class VTVisitor<T> : public T, public BaseVisitor
{
	protected:
		virtual void Visit(const T& v)
		{
			std::cout << "I saw a type I knew\n";
		}
};

////////

template <typename T1>
class TemplatedVisitor : public VTVisitor<T1> 
{
	public:
	TemplatedVisitor(IVisitable& v)
	{
		v.Accept(*this);
	}
};

int main(int argc, char* argv[])
{
	VisitableA a;
	VisitableB b;
  
	TemplatedVisitor<VisitableA> p_a(a);
	TemplatedVisitor<VisitableA> p_b(b);
}

