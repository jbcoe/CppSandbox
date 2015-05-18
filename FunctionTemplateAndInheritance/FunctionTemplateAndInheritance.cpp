#include <iostream>

struct Echoer
{
  virtual ~Echoer() {}
  virtual int echo(const int& i) = 0;
  virtual std::string echo(const std::string& s) = 0;
};

struct SimpleEchoer : Echoer
{
  template <class T>
  T echo(const T& t) { return t; }

  int echo(const int& i) { return echo<int>(i); }
  std::string echo(const std::string& i) { return echo<std::string>(i); }
};

int main(int argc, char* argv[])
{
  SimpleEchoer e;
}

