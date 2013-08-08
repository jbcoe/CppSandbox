#include <iostream>                                
#include <memory>                                
#include <vector>                                

class IVisitor;
class IVisitable
{
	public:
		virtual void Accept(IVisitor& v) const = 0;
		virtual ~IVisitable(){}
};

class A;
class B;
class C;

class IVisitor
{
	public:
		virtual void Visit(const A& a) = 0;
		virtual void Visit(const B& b) = 0;
		virtual void Visit(const C& b) = 0;
		~IVisitor(){}
};

class A : public IVisitable
{
	public:

		A(int i) : id_(i) {}

		void Accept(IVisitor& v) const
		{
			v.Visit(*this);
		}

		int A_ID() const { return id_; }
	
	private:
		int id_;
};

class B : public IVisitable
{
	public:

		B(int i) : id_(i) {}

		void Accept(IVisitor& v) const
		{
			v.Visit(*this);
		}

		int B_ID() const { return id_; }
	
	private:
		int id_;
};
  
class C : public IVisitable
{
	public:

		C(int i) : id_(i) {}

		void Accept(IVisitor& v) const
		{
			v.Visit(*this);
		}

		int C_ID() const { return id_; }
	
	private:
		int id_;
};

class NullVisitor : public IVisitor
{
		void Visit(const A& a) {}
		void Visit(const B& b) {}
		void Visit(const C& b) {}
};

template <typename Matched_T, typename MatchFunctor_T> 
struct Matcher : public NullVisitor
{
		Matcher(MatchFunctor_T f) : f_(f) {}

	const Matched_T* matched_ = nullptr;
	MatchFunctor_T f_;

  bool IsMatch(const IVisitable& o)
	{
		matched_ = nullptr;
		o.Accept(*this);
		return matched_ != nullptr;
	}

	void Visit(const Matched_T& m)
	{
		if ( f_(m) )
			matched_ = &m;
	}
};

template <typename Matched_T, typename MatchFunctor_T> 
Matcher<Matched_T, MatchFunctor_T> make_matcher(MatchFunctor_T f)
{
	return Matcher<Matched_T, MatchFunctor_T>(f);
}

int main(int argc, char* argv[])
{
	std::vector<std::unique_ptr<IVisitable>> objects;

  objects.push_back(std::unique_ptr<A>(new A(1)));
  objects.push_back(std::unique_ptr<B>(new B(2)));
  objects.push_back(std::unique_ptr<C>(new C(3)));

	auto b2Matcher = make_matcher<B>([](const B& b){ return b.B_ID() == 2; });
  for ( auto& p : objects)
	{
    if ( b2Matcher.IsMatch(*p) )
			std::cout << "Matched a B with id 2" << std::endl;
	}
}

