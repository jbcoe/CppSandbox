#include <iostream>
#include <exception>

template <typename Base>
class ThrowOnCopy
{
  Base base_;

public:
  ThrowOnCopy(Base b) : base_(std::move(b))
  {
  }

  ThrowOnCopy() : base_()
  {
  }

  class Exception : public std::exception
  {
  };

  ThrowOnCopy& operator=(ThrowOnCopy&&) = default;

  ThrowOnCopy(ThrowOnCopy&&) = default;

  ThrowOnCopy& operator=(const ThrowOnCopy&)
  {
    throw Exception();
  }

  ThrowOnCopy(const ThrowOnCopy&)
  {
    throw Exception();
  }

  Base& Unwrap()
  {
    return base_;
  }

  const Base& CUnwrap() const
  {
    return base_;
  }

  operator Base&()
  {
    return base_;
  }

  operator const Base&() const
  {
    return base_;
  }
};

class Empty
{
};

int main(int argc, char* argv[])
{
  ThrowOnCopy<Empty> e;

  try
  {
    auto c = e;
  }
  catch (ThrowOnCopy<Empty>::Exception&)
  {
    std::cout << "Exception thrown on copy as expected\n";
  }

  auto m = std::move(e);
}
