#include <iostream>
#include <thread>

int main(int argc, char* argv[])
{
  int count = 0;

  const size_t N = 100000;

  std::thread t1([&](){ for (int i=0;i<N;++i) ++count; });
  std::thread t2([&](){ for (int i=0;i<N;++i) ++count; });

  t1.join();
  t2.join();

  std::cout << count << std::endl;

  return 0;
}

