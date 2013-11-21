#include <iostream>
#include <memory>
#include <vector>

template <typename Get_F, typename Count_T = size_t>
class COMIterableProxy
{
public:
  typedef decltype(((Get_F*) nullptr)->operator()(0)) Object_T;

  COMIterableProxy(Get_F f, Count_T count) : m_f(std::move(f)), m_size(std::move(count)) {}

	template<typename IteratorValue_T>
  class FakeIterator
  {
  public:
    FakeIterator(Get_F f, Count_T index)
        : m_f{std::move(f)}, m_index{std::move(index)}
    {
    }

    FakeIterator<IteratorValue_T>& operator++()
    {
      ++m_index;
      return *this;
    }

    IteratorValue_T operator*()
    {
      return m_f(m_index);
    }

    FakeIterator<IteratorValue_T>& operator=(const FakeIterator<IteratorValue_T>& it) = default;
    
		FakeIterator(const FakeIterator<IteratorValue_T>& it) = default;

    bool operator==(const FakeIterator<IteratorValue_T>& it)
		{
			return m_index==it.m_index;
		}

    bool operator!=(const FakeIterator<IteratorValue_T>& it)
		{
			return m_index!=it.m_index;
		}

  private:
    Get_F m_f;
    Count_T m_index;
  };
                     
  typedef FakeIterator<Object_T> iterator;
  typedef FakeIterator<const Object_T> const_iterator;

	auto begin() { return iterator(m_f,Count_T{0});} 
	auto end() { return iterator(m_f,Count_T{m_size});} 

	auto begin() const { return const_iterator(m_f,Count_T{0});} 
	auto end() const { return const_iterator(m_f,Count_T{m_size});} 

private:
  Get_F m_f;
  Count_T m_size;
};

template<typename List_T>
auto make_iterable(List_T& list)
{
	List_T* pList = &list;
	auto accessor = [=](auto i){return pList->Get(i);};
	return COMIterableProxy<decltype(accessor)>(accessor, list.Count());
}

template <typename Object_T>
class ICOMList
{
public:
	virtual const Object_T* Get(size_t i) const = 0;
	virtual Object_T* GetEditable(size_t i) = 0;
	virtual size_t Count() const = 0;
	virtual ~ICOMList(){}
};

template <typename Object_T>
class CCOMListImpl : public ICOMList<Object_T>
{
public:	
	Object_T* Add() 
	{
		m_objects.push_back(std::unique_ptr<Object_T>(new Object_T{})); 
		return m_objects.back().get(); 
	}

	const Object_T* Get(size_t i) const override { return m_objects[i].get();}
	Object_T* GetEditable(size_t i) override { return m_objects[i].get();}
	size_t Count() const override { return m_objects.size(); } 

private:
	std::vector<std::unique_ptr<Object_T>> m_objects;
};


int main(int argc, char* argv[])
{
	CCOMListImpl<double> myDoubles;
  for(int i=0; i<10; ++i)
	{
		auto v = myDoubles.Add();
		*v = i;
	}

#ifndef RAW_COM_ITERATION
  CCOMListImpl<double>* pL = &myDoubles;
	for(auto x : make_iterable(myDoubles))
	{
		std::cout << *x << std::endl;
	}
#else
  for(int i=0; i<10; ++i)
	{
		std::cout << *myDoubles.Get(i) << std::endl;
	}
#endif
}
