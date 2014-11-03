// based on http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4082.pdf
// and      http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3525.pdf

#include <array>
#include <cassert>
#include <exception>
#include <sstream>
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
};

class new_and_delete_t : public memory_resource
{
  std::allocator<char> m_allocator;

public:
  virtual void* do_allocate(size_t bytes, size_t) override
  {
    return m_allocator.allocate(bytes);
  }

  virtual void do_deallocate(void* p, size_t bytes, size_t) override
  {
    m_allocator.deallocate(static_cast<char*>(p), bytes);
  }
};
                      
new_and_delete_t new_and_delete;
memory_resource* const default_resource = &new_and_delete;

template <class Tp>
class polymorphic_allocator
{
  memory_resource* m_resource;

public:

  typedef Tp value_type;

  polymorphic_allocator() : m_resource(default_resource) {}

  polymorphic_allocator(const polymorphic_allocator& a) = default;
  
  polymorphic_allocator(polymorphic_allocator&& a) = default;

  polymorphic_allocator(memory_resource* r) : m_resource(r) { assert(r); }
  
  polymorphic_allocator select_on_container_copy_construction() const
  {
    return polymorphic_allocator();
  }

  memory_resource* resource() const { return m_resource; }

  Tp* allocate(size_t n) 
  { 
    return (Tp*)m_resource->allocate(n * sizeof(Tp)); 
  }

  void deallocate(Tp* p, size_t n)
  {
    m_resource->deallocate(p, n * sizeof(Tp));
  }
};

template <size_t N>
class monotonic_buffer_resource : public memory_resource
{
  std::array<char,N> m_buffer;  
  void* m_next = nullptr;
  size_t m_remaining = 0;

public:

  monotonic_buffer_resource() : m_next(m_buffer.data()), m_remaining(N) 
  {
    m_buffer.fill(0);
    std::cout << "buffer constructed with size " << N << "\n";
  }

  monotonic_buffer_resource(const monotonic_buffer_resource&) = delete;

  virtual void* do_allocate(std::size_t sz, std::size_t alignment) override
  {
    std::cout << "Requested " << sz << " with alignment " << alignment << " with " << m_remaining << " remaining\n";
    auto initial_remaining = m_remaining;
    if (std::align(alignment, sz, m_next, m_remaining))
    {
      if ( initial_remaining != m_remaining )
      {
        std::cout << initial_remaining - m_remaining << " bytes used for alignment\n";
      }
      m_remaining -= sz;
      std::cout << sz << " bytes used for data\n";
      std::cout << m_remaining << " bytes remaining\n";
      return m_next;
    }
    else 
    {
      std::ostringstream ss;
      ss << "ERROR: Requested " << sz << " with alignment " << alignment << " but only " << m_remaining << " remaining";
      throw std::runtime_error(ss.str());
    }
  }
  
  virtual void do_deallocate(void*, std::size_t, std::size_t) override 
  {
  }
};

template <typename T>
using Vector = std::vector<T, polymorphic_allocator<T>>;

int main(int argc, char* argv[])
{
  try 
  {
    monotonic_buffer_resource<32> b;
    Vector<int> ints(6,0,&b);

    // Buffer is now fully used
    Vector<int> moreInts(ints);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return -1;
  }
}

