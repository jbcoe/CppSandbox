#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <mutex>

class MyClassWithLazyInitialization
{
public:
  MyClassWithLazyInitialization() {}
  const char* Name()
  {
    std::call_once(m_nameInitFlag, [this]()
                   {
      this->m_name = "Boris";
      std::cout << "Object has been named\n";
    });
    return m_name.c_str();
  }

private:
  std::once_flag m_nameInitFlag;
  std::string m_name;
};

int main(int argc, char* argv[])
{
  MyClassWithLazyInitialization o;

  std::mutex coutMutex;

  std::vector<std::thread> threads;
  for (size_t i = 0; i < 10; ++i)
  {
    threads.push_back(std::thread([&]
                                  {
                                    std::lock_guard<std::mutex> l(coutMutex);
                                    std::cout << o.Name() << "\n";
                                  }));
  }
  std::for_each(threads.begin(), threads.end(), [](std::thread& t)
                {
    t.join();
  });
}
