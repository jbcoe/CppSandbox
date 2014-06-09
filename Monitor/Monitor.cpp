// Based on Herb Sutter's talk at C++ and beyond 2012

#include <iostream>
#include <algorithm>
#include <string>
#include <thread>
#include <sstream>
#include <vector>

template <typename T>
class Monitor
{
  T t_;
  mutable std::mutex m_;

  operator const T&() const { return t_; }

  operator T&() { return t_; }

public:

  Monitor()=default;
  Monitor(const Monitor&)=delete;
  Monitor(Monitor&&)=delete;
  Monitor& operator=(const Monitor&)=delete;
  Monitor& operator=(Monitor&&)=delete;

  template <typename F>
  auto operator()(F f)
  {
    std::lock_guard<std::mutex> l(m_);
    return f(static_cast<T&>(*this));
  }

  template <typename F>
  auto operator()(F f) const
  {
    std::lock_guard<std::mutex> l(m_);
    return f(static_cast<const T&>(*this));
  }
};


int main(int argc, char* argv[])
{
  Monitor<std::stringstream> ss;

  std::vector<std::thread> threads;

  
	threads.emplace_back([&]()
                       {
                         ss([](auto& ss)
                                   {
                                     ss << "Hello 1 : "
                                        << std::this_thread::get_id()
                                        << std::endl;
                                   });
                       });
  
	threads.emplace_back([&]()
                       {
                         ss([](auto& ss)
                                   {
                                     ss << "Hello 2 : "
                                        << std::this_thread::get_id()
                                        << std::endl;
                                   });
                       });
  
	threads.emplace_back([&]()
                       {
                         ss([](auto& ss)
                                   {
                                     ss << "Hello 3 : "
                                        << std::this_thread::get_id()
                                        << std::endl;
                                   });
                       });
  
	threads.emplace_back([&]()
                       {
                         ss([](auto& ss)
                                   {
                                     ss << "Hello 4 : "
                                        << std::this_thread::get_id()
                                        << std::endl;
                                   });
                       });

  for (auto& t : threads)
  {
    t.join();
  }

	ss([](auto& ss){ std::cout << ss.str() << std::endl; });
}
