#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>

template <typename T>
class SynchronizedValue
{
  T t_;
  mutable std::shared_timed_mutex m_;

public:
  class Guard
  {
    const T* t_;
    std::shared_lock<std::shared_timed_mutex> l_;

  public:
    Guard(const T& t, std::shared_timed_mutex& m) : t_(&t), l_(m) {}

    const T* operator->() const { return t_; }
  };

  class EditGuard
  {
    T* t_;
    std::unique_lock<std::shared_timed_mutex> l_;

  public:
    EditGuard(const T& t, std::shared_timed_mutex& m) : t_(&t), l_(m) {}

    const T* operator->() const { return t_; }

    T* operator->() { return t_; }
  };
  
  SynchronizedValue() : t_() {}

  SynchronizedValue(const T& t) : t_(t) {}

  SynchronizedValue(T&& t) : t_(std::move(t)) {}

  Guard Access() const { return Guard(t_, m_); }
};

class MyClass
{
public:
  void Greet(const char* s) const
  {
    std::cout << '"' << greeting_ << '"' << " from " << s << std::endl;
  }

  void SetGreeting(const char* greeting)
  {
    greeting_ = greeting;
  }

  std::string greeting_ = "Hello";
};

int main(int argc, char* argv[])
{
  SynchronizedValue<MyClass> s;
  {
    auto g = s.Access();

    g->Greet("Main thread");
  }

  std::vector<std::thread> ts;

  for (size_t i = 0; i < 5; ++i)
  {
    ts.emplace_back([&,i]()
                    {
                      auto g = s.Access();
                      std::ostringstream ss;
                      ss << "Spawned thread " << i;
                      g->Greet(ss.str().c_str());
                    });
  }

  for(auto& t : ts ) 
  {
    t.join();
  }
}

