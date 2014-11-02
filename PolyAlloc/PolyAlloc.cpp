// based on http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4082.pdf
// 8.3-

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

class memory_resource
{
  static constexpr size_t max_align = alignof(max_align_t);

public:
  virtual ~memory_resource() {}

  void* allocate(size_t bytes, size_t alignment = max_align)
  {
    return do_allocate(bytes, alignment);
  }

  void deallocate(void* p, size_t bytes, size_t alignment = max_align)
  {
    do_deallocate(p, bytes, alignment);
  }

protected:
  virtual void* do_allocate(size_t bytes, size_t alignment) = 0;
  virtual void do_deallocate(void* p, size_t bytes, size_t alignment) = 0;
  // virtual bool do_is_equal(const memory_resource& other) const noexcept = 0;
};

struct malloc_and_free_t : memory_resource
{
  virtual void* do_allocate(size_t bytes, size_t) { return malloc(bytes); }
  virtual void do_deallocate(void* p, size_t, size_t) { return free(p); }
};

malloc_and_free_t malloc_and_free;

template <class Tp>
class polymorphic_allocator
{
  memory_resource* m_resource;

public:

  typedef Tp value_type;

  polymorphic_allocator() : m_resource(&malloc_and_free) {}

  polymorphic_allocator(memory_resource* r) : m_resource(r) { assert(r); }

  // polymorphic_allocator(const polymorphic_allocator& other) = default;

  // template <class U>
  //  polymorphic_allocator(const polymorphic_allocator<U>& other) noexcept;

  // polymorphic_allocator& operator=(const polymorphic_allocator& rhs) =
  // default;

  Tp* allocate(size_t n) { return (Tp*)m_resource->allocate(n * sizeof(Tp)); }

  void deallocate(Tp* p, size_t n)
  {
    m_resource->deallocate(p, n * sizeof(Tp));
  }

  // template <class T, class... Args>
  //  void construct(T* p, Args&&... args);

  template <class T>
  void destroy(T* p)
  {
    p->~T();
  }
};

class delegating_memory_resource : public memory_resource
{
  memory_resource* m_resource;

public:
  
  std::function<void(size_t,size_t)> m_on_allocate;
  std::function<void(void*,size_t,size_t)> m_on_deallocate;
  
  delegating_memory_resource(memory_resource* r) : m_resource(r) { assert(r); }

  virtual void* do_allocate(size_t bytes, size_t alignment) override
  {
    if ( m_on_allocate ) { m_on_allocate(bytes, alignment); }
    return m_resource->allocate(bytes, alignment);
  }

  virtual void do_deallocate(void* p, size_t bytes, size_t alignment) override
  {
    if ( m_on_deallocate ) { m_on_deallocate(p, bytes, alignment); }
    return m_resource->deallocate(p, bytes, alignment);
  }
};

int main(int argc, char* argv[])
{
  delegating_memory_resource mr(&malloc_and_free);
  mr.m_on_allocate = [](size_t bytes, size_t) { std::cout << "Allocated " << bytes << " bytes\n"; };
  
  std::vector<int, polymorphic_allocator<int>> ints(0,polymorphic_allocator<int>(&mr));
  ints.push_back(1);
  ints.push_back(2);
  ints.push_back(3);
  ints.push_back(4);
  ints.push_back(5);
  ints.push_back(6);
  ints.push_back(7);
  ints.push_back(8);

  for (const auto& i : ints)
  {
    std::cout << i << ' ';
  }
  std::cout << '\n';
}

