#include <boost/smart_ptr.hpp>
#include <vector>
#include <iostream>
#include <Common/Timer.h>

//////////////////////////////////////////////////

template <typename T>
void intrusive_ptr_add_ref(T* p) 
{ 
	++(p->refCount_); 
}
       
template <typename T>
void intrusive_ptr_release(T* p) 
{ 
	if ( ! --p->refCount_ ) delete p; 
}

//////////////////////////////////////////////////

template<typename T>
class CRefCounted
{
	friend void intrusive_ptr_add_ref<T>(T* p);
  friend void intrusive_ptr_release<T>(T* p);
	std::atomic<int> refCount_{0};	
};

//////////////////////////////////////////////////

class ClassWithVector : public CRefCounted<ClassWithVector>
{
public:
  ClassWithVector() : data_(1024) {}

private:
	std::vector<double> data_;
};

//////////////////////////////////////////////////

class ClassWithArray : public CRefCounted<ClassWithArray>
{
public:
  ClassWithArray() { std::fill(std::begin(data_), std::end(data_), 0); }

private:
	std::array<double,1024> data_;
};

//////////////////////////////////////////////////

class CheapClass : public CRefCounted<CheapClass> {}; 

//////////////////////////////////////////////////

template <typename T>
void RunDefaultConstructableObjectTimings(size_t objectCount, const char* objectName)
{
	std::cout << objectName << std::endl;

	{
		// No timing, warm up the heap
		for ( int i=0; i<1; ++i )
		{
			std::vector<std::shared_ptr<T>> objects;
			std::generate_n(std::back_inserter(objects), objectCount, []{return std::shared_ptr<T>(new T());});
		}
	}
	
	{
		auto t = make_timer("std::shared_ptr and new : ", objectName, " ", objectCount);
		std::vector<std::shared_ptr<T>> objects;
		std::generate_n(std::back_inserter(objects), objectCount, []{return std::shared_ptr<T>(new T());});
	}

	{
		auto t = make_timer("std::shared_ptr and make_shared : ", objectName, " ", objectCount);
		std::vector<std::shared_ptr<T>> objects;
		std::generate_n(std::back_inserter(objects), objectCount, []{return std::make_shared<T>();});
	}
	
	{
		auto t = make_timer("std::unique_ptr and new : ", objectName, " ", objectCount);
		std::vector<std::unique_ptr<T>> objects;
		std::generate_n(std::back_inserter(objects), objectCount, []{return std::unique_ptr<T>(new T());});
	}

	{
		auto t = make_timer("boost::shared_ptr and new : ", objectName, " ", objectCount);
		std::vector<boost::shared_ptr<T>> objects;
		std::generate_n(std::back_inserter(objects), objectCount, []{return boost::shared_ptr<T>(new T());});
	}
	
	{
		auto t = make_timer("boost::shared_ptr and make_shared : ", objectName, " ", objectCount);
		std::vector<boost::shared_ptr<T>> objects;
		std::generate_n(std::back_inserter(objects), objectCount, []{return boost::make_shared<T>();});
	}
	
	{
		auto t = make_timer("boost::intrusive_ptr : ", objectName, " ", objectCount);
		std::vector<boost::intrusive_ptr<T>> objects;
		std::generate_n(std::back_inserter(objects), objectCount, []{return boost::intrusive_ptr<T>(new T());});
	}

	std::cout << std::endl;
}

//////////////////////////////////////////////////

int main(int argc, char* argv[])
{
  RunDefaultConstructableObjectTimings<ClassWithVector>(1e5, "ClassWithVector");
  RunDefaultConstructableObjectTimings<ClassWithArray>(1e5, "ClassWithArray");
  RunDefaultConstructableObjectTimings<CheapClass>(2e6, "CheapClass");
}

