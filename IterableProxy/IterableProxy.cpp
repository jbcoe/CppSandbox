#include <iostream>
#include <memory>
#include <vector>

class ITiger{};
class IBear {};
class ILion {};

class IZoo
{
	public:
	
 	virtual ~IZoo() {}

	virtual size_t BearCount() const = 0;
	virtual const IBear& Bear(size_t index) const = 0;
	
	virtual size_t TigerCount() const = 0;
	virtual const ITiger& Tiger(size_t index) const = 0;
	
	virtual size_t LionCount() const = 0;
	virtual const ILion& Lion(size_t index) const = 0;
};

class Zoo : public IZoo
{
	public:

  Zoo() 
	{
		std::generate_n(std::back_inserter(m_lions), 10, [](){return std::unique_ptr<ILion>();});
	}

	size_t BearCount() const { return m_bears.size(); }
	const IBear& Bear(size_t index) const { return *m_bears[index]; }
	
	size_t TigerCount() const { return m_tigers.size(); }
	const ITiger& Tiger(size_t index) const { return *m_tigers[index]; }
	
	size_t LionCount() const { return m_lions.size(); }
	const ILion& Lion(size_t index) const { return *m_lions[index]; }

	private:
		std::vector<std::unique_ptr<ILion>> m_lions;
		std::vector<std::unique_ptr<ITiger>> m_tigers;
		std::vector<std::unique_ptr<IBear>> m_bears;
};

template <typename Object_t, typename Get_f, typename Count_f>
class IterableProxy
{
	public:

		IterableProxy(Get_f f_get, Count_f f_count) : m_f_get{f_get}, m_f_count{f_count} {} 

   class iterator
	 {
		 public:
      iterator(Get_f f_get, size_t index=0) : m_index(index), m_f_get{f_get} {}  
      
      const Object_t& operator * () { return *m_f_get(m_index); }
      iterator& operator++() { ++m_index; return *this; }
			bool operator == (const iterator& i) const { return m_index == i.m_index; }
			bool operator != (const iterator& i) const { return m_index != i.m_index; }

		private:
			size_t m_index;
	 		Get_f m_f_get;
	 };

	 iterator begin() { return iterator(m_f_get); }

	 iterator end() { return iterator(m_f_get, m_f_count()); }

	private:
	 Get_f m_f_get;
	 Count_f m_f_count;
};

template <typename Object_t, typename Get_f, typename Count_f>
IterableProxy <Object_t, Get_f, Count_f>
make_iterable_proxy(Get_f f_get, Count_f f_count)
{
	return IterableProxy <Object_t, Get_f, Count_f>(f_get, f_count);
}

int main(int argc, char* argv[])
{
	Zoo myZoo;
	auto iterableLions = make_iterable_proxy<ILion>(
			[&myZoo](size_t index){ return &myZoo.Lion(index); }, 
			[&myZoo](){ return myZoo.LionCount(); });

	for ( const auto& myLion : iterableLions )
	{
		std::cout << "Roar!" << std::endl;
	}
}

