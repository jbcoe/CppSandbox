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

template <typename Visitable_t>
class TemplatedVisitor : private BaseVisitor
{
	public:
		TemplatedVisitor(const IVisitable& v)
		{
			v.Accept(*this);
		}

	private:
		virtual void Visit(const Visitable_t& v)
		{
			std::cout << "I saw a type I knew\n";
		}
};

int main(int argc, char* argv[])
{
	VisitableA a;
	VisitableB b;

	TemplatedVisitor<VisitableA> p_a(a);
	TemplatedVisitor<VisitableA> p_b(b);
}

