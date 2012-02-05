#include <iostream>
#include <vector>

template <class T> class malloc_allocator
{
	public:
		typedef T                 value_type;
		typedef value_type*       pointer;
		typedef const value_type* const_pointer;
		typedef value_type&       reference;
		typedef const value_type& const_reference;
		typedef std::size_t       size_type;
		typedef std::ptrdiff_t    difference_type;

		template <class U> 
		struct rebind { typedef malloc_allocator<U> other; };

		malloc_allocator() 
		{
			std::cout << "Created a malloc_allocator" << std::endl;
		}

		malloc_allocator(const malloc_allocator&) 
		{
			std::cout << "Copied a malloc_allocator" << std::endl;
		}
		
		template <class U> 
		malloc_allocator(const malloc_allocator<U>&) {}
		
		~malloc_allocator() 
		{
			std::cout << "Deleted a malloc_allocator" << std::endl;
		}

		pointer address(reference x) const { return &x; }
		
		const_pointer address(const_reference x) const 
		{ 
			return x;
		}

		pointer allocate(size_type n, const_pointer = 0) 
		{
			void* p = std::malloc(n * sizeof(T));
			if (!p)
				throw std::bad_alloc();

			std::cout << "Allocated a block of memory of size " 
				<< n*sizeof(T) << std::endl;

			return static_cast<pointer>(p);
		}

		void deallocate(pointer p, size_type n) 
		{ 
			std::free(p); 
			std::cout << "Freed a block of memory of size "
				<< n*sizeof(T) << std::endl;
		}

		size_type max_size() const 
		{ 
			return static_cast<size_type>(-1) / sizeof(T);
		}

		void construct(pointer p, const value_type& x) 
		{
			std::cout << "Copy constructed an object" << std::endl;
			new(p) value_type(x); 
		}
		
		void construct(pointer p, value_type&& x) 
		{
			std::cout << "Move constructed an object" << std::endl;
			new(p) value_type(std::move(x)); 
		}
		
		void destroy(pointer p) 
		{ 
		  std::cout << "Destroyed an object" << std::endl;
			p->~value_type(); 
		}

	private:
		void operator=(const malloc_allocator&);
};

template<> class malloc_allocator<void>
{
	typedef void        value_type;
	typedef void*       pointer;
	typedef const void* const_pointer;

	template <class U> 
		struct rebind { typedef malloc_allocator<U> other; };
};

template <class T>
inline bool operator==(const malloc_allocator<T>&, 
		const malloc_allocator<T>&) 
{
	return true;
}

template <class T>
inline bool operator!=(const malloc_allocator<T>&, 
		const malloc_allocator<T>&) 
{
	return false;
}


class MyClass
{
	public:

		MyClass() 
		{ 
			std::cout << "My Class instance constructed" << std::endl;
		}

		MyClass(const MyClass&) 
		{
			std::cout << "My Class instance copy-constructed" << std::endl;
		}

		MyClass(MyClass&&) 
		{
			std::cout << "My Class instance move-constructed" << std::endl;
		}

	private:
		int m_x;
		int m_y;
		int m_z;
};

int main(int argc, char* argv[])
{
	std::vector<MyClass,malloc_allocator<MyClass>> myVector;
  myVector.reserve(10);
	std::cout << "============" << std::endl;
	myVector.push_back(MyClass());
	std::cout << "============" << std::endl;
	MyClass classInstance;
	myVector.push_back(classInstance);
	std::cout << "============" << std::endl;
}

