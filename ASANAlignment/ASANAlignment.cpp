#include <cstring>

//#define SAFE

int sum(const char* buffer) {
  int x;
  int y;
#ifndef SAFE
  x = *(int*)buffer;
  y = *(int*)(buffer + sizeof(int));
#else
  std::memcpy(&x, buffer, sizeof(int));
  std::memcpy(&y, buffer + sizeof(int), sizeof(int));
#endif
  return x + y;
}

int main(int argc, char* argv[]) {
  auto buffer = new char[3 * sizeof(int)];

  int x = 2;
  int y = 5;

#ifndef SAFE
  *(int*)(buffer + 4) = x;
  *(int*)(buffer + 4 + sizeof(int)) = y;
#else
  std::memcpy(buffer + 4, &x, sizeof(int));
  std::memcpy(buffer + 4 + sizeof(int), &y, sizeof(int));
#endif

  auto r = sum(&buffer[4]);

  delete[] buffer;

  return r - 7;
}
