#include <iostream>
#include <functional>

struct executor 
{
  using task = std::function<void()>;

  virtual ~executor() {}
  virtual void add_task(task t) = 0;
  virtual void wait() const = 0;
};

struct local_executor : executor
{
  virtual void add_task(task t) override
  {
    task();
  }
  
  virtual void wait() const override {}
};

int main(int argc, char* argv[])
{
}

