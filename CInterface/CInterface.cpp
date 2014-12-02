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


extern "C" {

Circle* Circle_new(double r) { return new (std::nothrow) Circle(r); }

void Shape_delete(Shape* shape) { delete shape; }

double Shape_area(Shape* shape) { return shape->area(); }

double Shape_perimiter(Shape* shape) { return shape->perimiter(); }
}

int main(int argc, char* argv[])
{
  auto c = Circle_new(10);
  std::cout << "Area=" << Shape_area(c) << "\n";
  std::cout << "Perimiter=" << Shape_perimiter(c) << "\n";
  Shape_delete(c);
}

