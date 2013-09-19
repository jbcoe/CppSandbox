#include <boost/regex.hpp>
#include <iostream>
#include <sstream>

const char* illegalCharacters = "(\\[)|(\\])|"
                                "(<)|(>)|"
                                "(\\*)|"
                                "(&)|"
                                "(\\t)|"
                                "(\\^)|"
                                "(%)|"
                                "(#)|"
                                "(£)|"
                                "(@)|"
                                "(!)|"
                                "(:)|"
                                "(;)|"
                                "(\\\")|"
                                "(')|"
                                "(\\$)|"
                                "(/)|"
                                "(\\))|"
                                "(\\()|"
                                "(\\\\)|"
                                "( )";

int main(int argc, char* argv[])
{
  std::string badFilename = "[]<>*&^%$£@!#()\"':;\\/ \t";
  std::cout << "INITIAL   __" << badFilename << "__" << std::endl;

  boost::regex exp(illegalCharacters);

  std::stringstream ss;

  std::ostream_iterator<char> oi(ss);

  boost::regex_replace(oi, badFilename.begin(), badFilename.end(), exp, "X",
                       boost::match_default | boost::format_all);

  std::string corrected(ss.str());

  std::cout << "FINAL __" << corrected << "__" << std::endl;
}
