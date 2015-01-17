// http://blogs.msdn.com/b/oldnewthing/archive/2015/01/16/10586692.aspx
#include <cstdint>
#include <array>
#include <iostream>
#include <Common/Timer.h>

class Buffer
{
public:

  void StoreInt64(int64_t t)
  {
    *reinterpret_cast<int64_t*>(data_) = t;
  }

private:
  char data_[sizeof(int64_t)];
};

template<size_t N>
class MisalignedBuffer
{
public:

  void StoreInt64(int64_t t)
  {
    *reinterpret_cast<int64_t*>(data_) = t;
  }

private:
  char padding_[N];
  char data_[sizeof(int64_t)];
};

int main(int argc, char* argv[]) 
{
  const size_t LOOPS = 1e9;

  {
    auto t = Timer("Buffer");
    Buffer b;
    for(size_t i =0; i<LOOPS; ++i)
    {
      b.StoreInt64(12345);
    }
  }

  {
    auto t = Timer("MisalignedBuffer<1>");
    MisalignedBuffer<1> b;
    for(size_t i =0; i<LOOPS; ++i)
    {
      b.StoreInt64(12345);
    }
  }
  
  {
    auto t = Timer("MisalignedBuffer<2>");
    MisalignedBuffer<2> b;
    for(size_t i =0; i<LOOPS; ++i)
    {
      b.StoreInt64(12345);
    }
  }
  
  {
    auto t = Timer("MisalignedBuffer<3>");
    MisalignedBuffer<3> b;
    for(size_t i =0; i<LOOPS; ++i)
    {
      b.StoreInt64(12345);
    }
  }

  {
    auto t = Timer("MisalignedBuffer<4>");
    MisalignedBuffer<4> b;
    for(size_t i =0; i<LOOPS; ++i)
    {
      b.StoreInt64(12345);
    }
  }
}

