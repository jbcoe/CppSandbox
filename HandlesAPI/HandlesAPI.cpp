#include <iostream>
#include <memory>
#include <unordered_map>

using std::swap;

//
// The classes

struct Shape
{
  virtual ~Shape()
  {
  }
  virtual double area() const = 0;
  virtual double perimeter() const = 0;
};

static const double pi = 3.14159265359;

class Circle : public Shape
{
  const double radius_;

public:
  double area() const override
  {
    return pi * radius_ * radius_;
  }

  double perimeter() const override
  {
    return 2 * pi * radius_;
  }

  Circle(double r) : radius_(r)
  {
  }
};

//
// The C bridge

extern "C" {

void* Circle_new(double r)
{
  return new (std::nothrow) Circle(r);
}

void Shape_delete(const void* shape)
{
  delete ((const Shape*)shape);
}

double Shape_area(const void* shape)
{
  return ((const Shape*)shape)->area();
}

double Shape_perimeter(const void* shape)
{
  return ((const Shape*)shape)->perimeter();
}
}

//
// The handles bridge

using HANDLE = const char*;

enum RC 
{
  LOOKUP_FAIL = -999,
  SUCCESS = 0
};

struct TableEntry
{
  void* obj_ = nullptr;
  void (*del_)(void*) = nullptr;

  TableEntry& operator = (TableEntry&& t)
  {
    swap(t.obj_, obj_);
    swap(t.del_, del_);
    t.~TableEntry();
  }

  ~TableEntry()
  {
    if ( del_ ) del_(obj_);
  }
};

std::unordered_map<std::string, TableEntry> handle_table;

struct LookupFailureException : public std::runtime_error
{
  LookupFailureException(HANDLE handle) : std::runtime_error(handle) {}
};

void* Lookup(HANDLE handle)
{
  auto find_it = handle_table.find(handle);
  if ( find_it != handle_table.end() )
  {
    return find_it->second.obj_;
  }
  throw LookupFailureException(handle);
}

extern "C" {

void CircleH_new(HANDLE circle, double r, int* rc)
{
}

void ShapeH_delete(HANDLE shape, int* rc)
{
  try
  {
    Shape_delete(Lookup(shape));
  }
  catch(const LookupFailureException& e)
  {
    *rc=LOOKUP_FAIL;
  }
}

double ShapeH_area(HANDLE shape, int* rc)
{
  try
  { 
    *rc = SUCCESS;
    return Shape_area(Lookup(shape));
  }
  catch(const LookupFailureException& e)
  {  
    *rc=LOOKUP_FAIL;
    return 0.0; 
  }
}

double ShapeH_perimeter(HANDLE shape, int* rc)
{
  try
  {
    *rc = SUCCESS;
    return Shape_perimeter(Lookup(shape));
  }
  catch(const LookupFailureException& e)
  {
    *rc=LOOKUP_FAIL;
    return 0.0;
  }
}
}

//
// The other side

class CircleHandle
{
  const void* c_;

public:
  ~CircleHandle()
  {
    Shape_delete(c_);
  }

  CircleHandle(double r) : c_(Circle_new(r))
  {
    if (!c_)
    {
      throw std::runtime_error("Failed to build Circle");
    }
  }
  double area() const
  {
    return Shape_area(c_);
  }

  double perimiter() const
  {
    return Shape_perimeter(c_);
  }
};

int main(int argc, char* argv[])
{
  auto c = CircleHandle(10);
  std::cout << "Area=" << c.area() << "\n";
  std::cout << "Perimiter=" << c.perimiter() << "\n";
}
