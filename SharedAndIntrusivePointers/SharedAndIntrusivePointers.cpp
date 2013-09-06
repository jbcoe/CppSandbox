#include <boost/smart_ptr.hpp>
#include <vector>
#include <iostream>
#include <Common/Timer.h>

//////////////////////////////////////////////////

struct ClassWithVector
{
  ClassWithVector() : refCount_(0), data_(1024) 
	{
	}
	~ClassWithVector()
	{
	}

	std::atomic<int> refCount_;
	
	std::vector<double> data_;
};

void intrusive_ptr_add_ref(ClassWithVector* p) 
{ 
	++(p->refCount_); 
}

void intrusive_ptr_release(ClassWithVector* p) 
{ 
	if ( ! --p->refCount_ ) delete p; 
}

//////////////////////////////////////////////////

struct ClassWithArray
{
  ClassWithArray() : refCount_(0)
	{
	}
	~ClassWithArray()
	{
	}

	std::atomic<int> refCount_;
	
	std::array<double,1024> data_;
};

void intrusive_ptr_add_ref(ClassWithArray* p) 
{ 
	++(p->refCount_); 
}

void intrusive_ptr_release(ClassWithArray* p) 
{ 
	if ( ! --p->refCount_ ) delete p; 
}

//////////////////////////////////////////////////
	
template <typename T>
void RunDefaultConstructableObjectTimings(size_t objectCount, const char* objectName)
{
	{
		// No timing, warm up the heap
		for ( int i=0; i<10; ++i )
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
	
	std::cout << std::endl;

	{
		auto t = make_timer("std::shared_ptr and make_shared : ", objectName, " ", objectCount);
		std::vector<std::shared_ptr<T>> objects;
		std::generate_n(std::back_inserter(objects), objectCount, []{return std::make_shared<T>();});
	}
	
	std::cout << std::endl;
	
	{
		auto t = make_timer("std::unique_ptr and new : ", objectName, " ", objectCount);
		std::vector<std::unique_ptr<T>> objects;
		std::generate_n(std::back_inserter(objects), objectCount, []{return std::unique_ptr<T>(new T());});
	}

	std::cout << std::endl;
	
	{
		auto t = make_timer("boost::shared_ptr and new : ", objectName, " ", objectCount);
		std::vector<boost::shared_ptr<T>> objects;
		std::generate_n(std::back_inserter(objects), objectCount, []{return boost::shared_ptr<T>(new T());});
	}
	
	std::cout << std::endl;

	{
		auto t = make_timer("boost::shared_ptr and make_shared : ", objectName, " ", objectCount);
		std::vector<boost::shared_ptr<T>> objects;
		std::generate_n(std::back_inserter(objects), objectCount, []{return boost::make_shared<T>();});
	}
	
	std::cout << std::endl;
	
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
  RunDefaultConstructableObjectTimings<ClassWithVector>(5e5, "ClassWithVector");
  RunDefaultConstructableObjectTimings<ClassWithArray>(1e6, "ClassWithArray");
}

