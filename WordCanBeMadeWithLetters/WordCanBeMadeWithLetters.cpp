#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
  if (argc == 1)
  {
    cerr << "Needs a letter list as first argument and a word list piped from "
            "stdin" << endl;
    exit(1);
  }

  size_t minLength = 0;
  if (argc == 3) minLength = atoi(argv[2]);

  string letters = argv[1];
  sort(letters.begin(), letters.end());
  transform(letters.begin(), letters.end(), letters.begin(), [](char c)
            { return tolower(c); });

  std::vector<string> results;

  std::istream_iterator<string> is(cin);
  std::istream_iterator<string> is_end;

  auto letter_end = letters.end();

  for_each(is, is_end, [&](const std::string& _s)
           {
    if (_s.size() > letters.size()) return;
    if (_s.size() < minLength) return;

    auto s(_s);
    sort(s.begin(), s.end());
    transform(s.begin(), s.end(), s.begin(), [](char c)
              { return tolower(c); });

    auto letter_it = letters.begin();

    for (auto it = s.begin(); it != s.end(); ++it)
    {
      auto find_letter = find(letter_it, letter_end, *it);
      if (find_letter == letter_end) return;
      letter_it = find_letter;
      ++letter_it;
    }

    results.push_back(_s);
  });

  sort(results.begin(), results.end(),
       [](const string& s1, const string& s2) -> bool
       {
    if (s1.size() < s2.size()) return true;

    if (s2.size() < s1.size()) return false;

    return s1 < s2;
  });

  results.erase(unique(results.begin(), results.end()), results.end());

  std::ostream_iterator<string> os(cout, "\n");
  std::copy(results.begin(), results.end(), os);
}
