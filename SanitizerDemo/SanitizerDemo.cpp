#include <iostream>

int main(int argc, char* argv[])
{
  double* n = new double[100];
  n[100] = 22; // out of bounds
  delete n; // should be delete[]
  n[0] = 5; // use after free
}

