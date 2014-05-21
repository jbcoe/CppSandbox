#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>

std::ostream* g_logStream = &std::cout;

std::mutex g_logMutex;

class LogLocker
{
public:
  LogLocker() : l(g_logMutex) {}

private:
  std::unique_lock<std::mutex> l;
};

template <typename T>
std::ostream& operator<<(LogLocker&&, T&& t)
{
  return *g_logStream << std::forward<T>(t);
}

enum class LEVEL
{ INFO = 3,
  WARNING = 2,
  ERROR = 1,
  CRITICAL = 0 } g_logLevel = LEVEL::CRITICAL;

void SetLogLevel(LEVEL logLevel) { g_logLevel = logLevel; }

#define LOG(level)                                                             \
  if (g_logLevel < level)                                                      \
    ;                                                                          \
  else                                                                         \
  LogLocker()

int main(int argc, char* argv[])
{
  std::vector<LEVEL> logLevels = {LEVEL::INFO, LEVEL::WARNING, LEVEL::ERROR,
                                  LEVEL::CRITICAL};
  for (auto level : logLevels)
  {
    SetLogLevel(level);

    LOG(LEVEL::INFO) << "It's getting warm" << std::endl;
    LOG(LEVEL::WARNING) << "It's getting dangerously hot" << std::endl;
    LOG(LEVEL::ERROR) << "I'm starting to singe a bit" << std::endl;
    LOG(LEVEL::CRITICAL) << "I am on fire!\n" << std::endl;
  }

  std::thread t1([]()
                 {
                   for (int i = 0; i < 50; ++i)
                   {
                     LOG(LEVEL::CRITICAL) << "Are we locked?" << std::endl;
                     std::this_thread::sleep_for(
                         std::chrono::milliseconds(100));
                   }
                 });

  std::thread t2([]()
                 {
                   for (int i = 0; i < 50; ++i)
                   {
                     LOG(LEVEL::CRITICAL) << "Are we really locked?"
                                          << std::endl;
                     std::this_thread::sleep_for(
                         std::chrono::milliseconds(200));
                   }
                 });

  t1.join();
  t2.join();

  return 0;
}
