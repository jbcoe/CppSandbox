#include <iostream>
#include <cassert>
#include <memory>
#include <unordered_map>

using std::swap;

//
// The classes

struct CountInstances
{
  CountInstances() { ++instanceCount; std::cout << "Instances: " << instanceCount << "\n"; }
  ~CountInstances() { --instanceCount; std::cout << "Instances: " << instanceCount << "\n"; }

  CountInstances(const CountInstances&)=delete;
  CountInstances(CountInstances&&)=delete;
  
  CountInstances& operator=(const CountInstances&)=delete;
  CountInstances& operator=(CountInstances&&)=delete;
  
  static size_t instanceCount;
};

size_t CountInstances::instanceCount = 0;

struct Shape : CountInstances
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
  void (*del_)(const void*) = nullptr;

  TableEntry& operator=(TableEntry&& t)
  {
    assert(&t != this);
    swap(t.obj_, obj_);
    swap(t.del_, del_);
    return *this;
  }

  ~TableEntry()
  {
    if (del_) del_(obj_);
  }
};

class HandleTable // not thread-safe
{

  std::unordered_map<std::string, TableEntry> handle_table;

public:
  struct LookupFailureException : public std::runtime_error
  {
    LookupFailureException(HANDLE handle) : std::runtime_error(handle)
    {
    }
  };

  void InsertOrOverwrite(HANDLE handle, void* object,
                         void (*deleter)(const void*))
  {
    TableEntry t;
    t.obj_ = object;
    t.del_ = deleter;
    handle_table[handle] = std::move(t);
  }

  void* Lookup(HANDLE handle) const
  {
    auto find_it = handle_table.find(handle);
    if (find_it != handle_table.end())
    {
      return find_it->second.obj_;
    }
    throw LookupFailureException(handle);
  }

  void* TryLookup(HANDLE handle) const
  {
    auto find_it = handle_table.find(handle);
    if (find_it != handle_table.end())
    {
      return find_it->second.obj_;
    }
    return nullptr;
  }
};

static HandleTable handle_table;

extern "C" {

void CircleH_new(HANDLE circle, double r, int* rc)
{
  handle_table.InsertOrOverwrite(circle, Circle_new(r), Shape_delete);
  *rc = SUCCESS;
}

void ShapeH_delete(HANDLE shape, int* rc)
{
  try
  {
    *rc = SUCCESS;
    Shape_delete(handle_table.Lookup(shape));
  }
  catch (const HandleTable::LookupFailureException& e)
  {
    *rc = LOOKUP_FAIL;
  }
}

double ShapeH_area(HANDLE shape, int* rc)
{
  try
  {
    *rc = SUCCESS;
    return Shape_area(handle_table.Lookup(shape));
  }
  catch (const HandleTable::LookupFailureException& e)
  {
    *rc = LOOKUP_FAIL;
    return 0.0;
  }
}

double ShapeH_perimeter(HANDLE shape, int* rc)
{
  try
  {
    *rc = SUCCESS;
    return Shape_perimeter(handle_table.Lookup(shape));
  }
  catch (const HandleTable::LookupFailureException& e)
  {
    *rc = LOOKUP_FAIL;
    return 0.0;
  }
}
}

int main(int argc, char* argv[])
{
  int rc;
  auto name = "myCircle";
  
  CircleH_new(name, 10, &rc);
  if (rc != SUCCESS)
  {
    std::cerr << "Failed to construct circle\n";
    return -1;
  }

  CircleH_new(name, 10, &rc);
  if (rc != SUCCESS)
  {
    std::cerr << "Failed to construct circle\n";
    return -1;
  }
  
  double area = ShapeH_area(name, &rc);
  if (rc != SUCCESS)
  {
    std::cerr << "Failed to get circle area\n";
    return -1;
  }
  std::cout << "Area=" << area << "\n";

  double perimeter = ShapeH_perimeter(name, &rc);
  if (rc != SUCCESS)
  {
    std::cerr << "Failed to get circle perimeter\n";
    return -1;
  }
  std::cout << "Perimeter=" << perimeter << "\n";
}

