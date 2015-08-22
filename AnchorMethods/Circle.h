#include "Shape.h"

class Circle : public Shape
{
  double radius_;
  public:
#ifdef ANCHORS  
  virtual ~Circle();
#else
  virtual ~Circle(){}
#endif
  Circle(double radius):radius_(radius){}
  double Area() const { return 3.14159*radius_*radius_; }
  double Perimeter() const { return 2.0*3.14159*radius_; }
};
