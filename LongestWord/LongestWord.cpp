#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <string>

int main(int argc, char* argv[])
{
  std::istream_iterator<std::string> is(std::cin);
  std::istream_iterator<std::string> is_end;

  std::vector<std::string> vWords;

  std::copy(is, is_end, std::back_inserter(vWords));

  // Sort by length so that longest word comes first
  std::sort(vWords.rbegin(), vWords.rend(),
            [](const std::string& s1, const std::string& s2)
            {
    if (s1.size() < s2.size()) return true;
    if (s2.size() < s1.size()) return false;
    return s1 < s2;
  });

  auto find_it = std::find_if(vWords.begin(), vWords.end(), [](std::string s)
                              {
    std::sort(s.begin(), s.end());
    auto it_uend = std::unique(s.begin(), s.end());
    return it_uend == s.end();
  });

  if (find_it != vWords.end()) std::cout << *find_it << std::endl;
}
