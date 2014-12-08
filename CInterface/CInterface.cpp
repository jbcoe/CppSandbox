#include <iostream>
#include <memory>

struct Shape
{
  virtual ~Shape() {}
  virtual double area() const = 0;
  virtual double perimiter() const = 0;
};

static const double pi = 3.14159265359;

class Circle : public Shape
{
  const double radius_;

public:
  double area() const override { return pi * radius_ * radius_; }

  double perimiter() const override { return 2 * pi * radius_; }

  Circle(double r) : radius_(r) {}
};

//
// The bridge

extern "C" {

void* Circle_new(double r) { return new (std::nothrow) Circle(r); }

void Shape_delete(const void* shape) { delete ((const Shape*)shape); }

double Shape_area(const void* shape) { return ((const Shape*)shape)->area(); }

double Shape_perimiter(const void* shape)
{
  return ((const Shape*)shape)->perimiter();
}
}

//
// The other side

class CircleHandle
{
  const void* c_;

public:
  ~CircleHandle() { Shape_delete(c_); }

  CircleHandle(double r) : c_(Circle_new(r))
  {
    if (!c_)
    {
      throw std::runtime_error("Failed to build Circle");
    }
  }
  double area() const { return Shape_area(c_); }

  double perimiter() const { return Shape_perimiter(c_); }
};

int main(int argc, char* argv[])
{
  auto c = CircleHandle(10);
  std::cout << "Area=" << c.area() << "\n";
  std::cout << "Perimiter=" << c.perimiter() << "\n";
}
