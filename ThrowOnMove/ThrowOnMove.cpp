#include <iostream>
#include <exception>

template <typename Base>
class ThrowOnMove
{
  Base base_;

public:
    
  ThrowOnMove(Base b) : base_(std::move(b)) {}
  
  ThrowOnMove() : base_() {}

  class Exception : public std::exception
  {
  };

  ThrowOnMove& operator=(ThrowOnMove&) = default;
  
  ThrowOnMove(ThrowOnMove&) = default;
  
  ThrowOnMove& operator=(ThrowOnMove&&)
  {
    throw Exception();
  }
  
  ThrowOnMove(ThrowOnMove&&)
  {
    throw Exception();
  }

  Base& Unwrap() { return base_; }
  
  const Base& CUnwrap() const { return base_; }
  
  operator Base& () { return base_; }
  
  operator const Base& () const { return base_; }
};

class Empty {};

int main(int argc, char* argv[])
{
  ThrowOnMove<Empty> e;

  auto c = e;
  try
  {
    auto m = std::move(e);
  }
  catch(ThrowOnMove<Empty>::Exception&)
  {
    std::cout << "Exception thrown on move as expected\n";
  }

}

