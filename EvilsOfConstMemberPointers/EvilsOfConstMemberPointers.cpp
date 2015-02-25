#include <iostream>

struct Data
{
  void mutateMe()
  {
    x = true;
  }

  bool x = false;
};


struct DataObserver
{
  DataObserver(Data& d_) : d(&d_)
  {
  }

  void ThreadSafeConstMethod() const
  {
    d->mutateMe();
  }

  Data* d;
};

int main()
{
  Data d;
  const DataObserver o(d);

  std::cout << std::boolalpha << d.x << std::endl;

  o.ThreadSafeConstMethod();

  std::cout << std::boolalpha << d.x << std::endl;
}
