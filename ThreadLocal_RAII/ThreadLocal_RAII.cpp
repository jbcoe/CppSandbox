#include <iostream>
#include <thread>
#include <Common/ScopedThread.h>

std::atomic<int> totalLiveThreadCount(0);
std::atomic<int> totalCreatedThreadCount(0);

struct ThreadCountInfo
{
  ThreadCountInfo()
  {
    ++totalLiveThreadCount;
    ++totalCreatedThreadCount;
  }

  ~ThreadCountInfo() { --totalLiveThreadCount; }

  void Init() {}
};

thread_local ThreadCountInfo tcInfo = ThreadCountInfo{};

std::atomic<bool> keepRunning(true);

int main(int argc, char* argv[])
{

  ScopedThread t1([]
                  {
                    tcInfo.Init();
                    while (keepRunning) std::this_thread::yield();
                  });
  ScopedThread t2([]
                  {
                    tcInfo.Init();
                    while (keepRunning) std::this_thread::yield();
                  });
  ScopedThread t3([]
                  {
                    tcInfo.Init();
                    while (keepRunning) std::this_thread::yield();
                  });
  ScopedThread dt1([]
                   { tcInfo.Init(); }); // Nothing to do

  while (totalCreatedThreadCount != 4 && totalLiveThreadCount != 3)
    ; // Loop

  std::cout << "Total threads spawned: " << totalCreatedThreadCount
            << "\nTotal live threads: " << totalLiveThreadCount << std::endl;

  keepRunning = false;
}
