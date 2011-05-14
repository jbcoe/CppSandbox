#include <iostream>

#ifdef USE_BOOST

#include <boost/regex.hpp>

#else

#include <regex>

#endif

#include <string>
     
#ifdef USE_BOOST
namespace re = boost;
#else
namespace re = std;
#endif

int main(int argc, char* argv[])
{
	try 
	{
		re::cmatch res;
		std::string str = "<h2>Egg prices</h2>";
		re::regex rx(">([^<]+)");
		re::regex_search(str.c_str(), res, rx);
		std::cout << res[1] << "\n";
	}
	catch ( const std::exception e)
	{
		std::cerr << e.what() << std::endl;
	}
}

