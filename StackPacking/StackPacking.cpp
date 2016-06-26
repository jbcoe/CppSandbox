#include <cstdio>

int main(int argc, char**)
{
  int* p = &argc;
  if(argc < 2 )
  {
    int x = argc-1;
    p = &x;
  }
  int y = argc+1;
  printf("%d %d\n", y, *p);
  return 0;
}

