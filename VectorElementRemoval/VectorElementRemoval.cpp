#include <vector>
#include <iostream>
#include <algorithm>

#ifndef REMOVE_VALUES
#ifndef CHECK_VALUES
#error "define one of REMOVE_VALUES or CHECK_VALUES"
#endif
#endif

#ifdef REMOVE_VALUES
#ifdef CHECK_VALUES
#error "define either REMOVE_VALUES or CHECK_VALUES"
#endif
#endif

int main()
{
  std::vector<int> myValues;
  for (long int i = 0L; i < 10000000L; ++i)
  {
    myValues.push_back(i % 3);
  }

  int iOriginalSize = myValues.size();
#ifdef REMOVE_VALUES
  myValues.erase(std::remove_if(myValues.begin(), myValues.end(),
                                [](int i)
                                {
                                  return i == 2;
                                }),
                 myValues.end());
#endif

  const int iterations = 100;
  for (int iteration = 0; iteration < iterations; ++iteration)
  {
    int sum = 0;
    for (unsigned int i = 0; i < myValues.size(); ++i)
    {
#ifdef CHECK_VALUES
      if (myValues[i] != 2)
      {
#endif
        sum += myValues[i];
#ifdef CHECK_VALUES
      }
#endif
    }
  }
}
