#include <iostream>
#include <tuple>

using namespace std;

tuple<int, int, double> ReturnTuple() { return make_tuple(1, 2, 3.14159); }

int main(int argc, char* argv[])
{
  int myInt = 0;
  double myDouble = 0.0;

  tie(myInt, ignore, myDouble) = ReturnTuple();

  cout << myInt << " " << myDouble << endl;
}
