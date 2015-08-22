struct Shape
{
#ifdef ANCHORS  
  virtual ~Shape();
#else
  virtual ~Shape(){}
#endif
  virtual double Area() const = 0;
  virtual double Perimeter() const = 0;
};
