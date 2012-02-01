#include <utility>
#include <iostream>
#include <map>

template<
	typename Key_t, 
	typename Value_t,
	typename Predicate_t = std::less<Key_t> >
class non_copyable_map : public std::map<Key_t,Value_t,Predicate_t>
{
  typedef std::map<Key_t,Value_t,Predicate_t> BaseType;

	public:
		non_copyable_map() { }
    
		non_copyable_map(non_copyable_map&& t) : BaseType(std::move(t)) 
		{
		}
    
    non_copyable_map& operator = (non_copyable_map&& t)
		{
			if ( this != &t )
			{
				std::swap<BaseType>(*this,t);
			}
			return *this;
		}
    
	private:
		non_copyable_map(const non_copyable_map&);
		non_copyable_map& operator = (const non_copyable_map&);
};

int main(int argc, char* argv[])
{
	non_copyable_map<int, non_copyable_map<int, int> > nestedMap;
	non_copyable_map<int,int> inner;
	inner[3]=4;
	nestedMap[2] = std::move(inner);
	
	auto find_inner = nestedMap.find(2);
	if ( find_inner != nestedMap.end() )
	{
		std::cout << "Found inner map " << std::endl;
	
		auto find_value = find_inner->second.find(3);

		//This HAS to be a reference otherwise compilation fails
		//Failing to compile when accidentally copying maps is the aim
		auto& giveMeAMap = find_inner->second;
		
		giveMeAMap[4] = 7;
		
		if ( find_value != find_inner->second.end() )
		{
			std::cout << "Found value " << find_value->second << std::endl;
		}
	}
}

