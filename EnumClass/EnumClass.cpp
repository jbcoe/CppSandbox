#include <iostream>

enum class Day
{
  Monday,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday,
  Sunday
};

enum class Month
{
  January,
  February,
  March,
  April,
  May,
  June,
  July,
  August,
  September,
  October,
  November,
  December
};

int main(int argc, char* argv[])
{
  Month eMonth;
  Day eDay;

  // No compilation as types are different
  // if ( eMonth == eDay ) {}

  eMonth = Month::March;
  eDay = Day::Wednesday;
}
