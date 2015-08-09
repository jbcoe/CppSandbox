#include <iostream>
#include <cassert>
#include <atomic>
#include <thread>
#include <vector>
#include <functional>

struct executor 
{
  using task = std::function<void()>;

  virtual void add_task(task t) = 0;
  virtual void wait() const = 0;

protected:
  ~executor()
  {
  }
};

struct local_executor_view : executor
{
  private:
    std::atomic<unsigned> tasks_;
    std::atomic<unsigned> completed_tasks_;
    executor& e_;
    unsigned sleep_time_; // sleep time in ms
  
  public:
    local_executor_view(executor& e, unsigned sleep_time = 100)
        : e_(e), sleep_time_(sleep_time)
    {
      completed_tasks_ = 0;
    }

    local_executor_view(local_executor_view&& ) = delete;
    local_executor_view(const local_executor_view& ) = delete;

    void add_task(task t)
    {
      ++tasks_;
      e_.add_task([ this, t = std::move(t) ]
                       {
                         t();
                         ++completed_tasks_;
                       });
    }

    void wait() const
    {
      while (completed_tasks_ < tasks_ )
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_));
      }
    }

    ~local_executor_view()
    {
      wait();
    }
};

struct simple_executor : executor
{
  std::vector<task> tasks_;
  void add_task(task t) { t(); }
  void wait() const { }
};

int main(int argc, char* argv[])
{
  simple_executor e;
  local_executor_view ve(e);
  ve.add_task([]{});
  ve.add_task([]{});
  ve.add_task([]{});
  ve.add_task([]{});
  ve.wait();
  e.wait();
}

