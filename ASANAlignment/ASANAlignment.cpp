#include <iostream>

int sum(const char* buffer)
{
  const int* values = (const int*)(buffer);
  
  return values[0] + values[1];
}

int main(int argc, char* argv[])
{
  auto buffer = new char[3*sizeof(int)];
 
  int* xs = (int*)&buffer[3];
  xs[0] = 2;
  xs[1] = 5;

  std::cout << sum(&buffer[3]) << std::endl;
  
  delete [] buffer;
}


