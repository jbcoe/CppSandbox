#include <iostream>
#include <algorithm>
#include <iterator>
#include <memory>
#include <vector>

class IObject
{
public:
  double m_value = 0;
};

class IListOfObjects
{
public:
  class proxy
  {
  public:
    explicit proxy(IObject* pObject) : m_pObject(pObject)
    {
    }
    void operator=(const proxy&) = delete;
    IObject* operator->()
    {
      return m_pObject;
    }

  private:
    IObject* m_pObject;
  };

  class iterator
  {
  public:
    typedef size_t difference_type;
    typedef size_t size_type;
    typedef proxy value_type;
    typedef proxy* pointer;
    typedef proxy& reference;

    bool operator==(const iterator& that) const
    {
      return m_position == that.m_position;
    }
    bool operator!=(const iterator& that) const
    {
      return m_position != that.m_position;
    }
    const iterator& operator++()
    {
      ++m_position;
      return *this;
    }
    iterator(const iterator& it) : m_list(it.m_list), m_position(it.m_position)
    {
    }
    iterator(IListOfObjects& list, size_t position)
        : m_list(list), m_position(position)
    {
    }

    proxy operator*()
    {
      return proxy(m_list.get(m_position));
    }

  private:
    IListOfObjects& m_list;
    size_t m_position;
  };

  iterator begin()
  {
    return iterator(*this, 0);
  }

  iterator end()
  {
    return iterator(*this, this->count());
  }

  virtual size_t count() const = 0;

  virtual const IObject* get(size_t i) const = 0;

  virtual IObject* get(size_t i) = 0;
};

class ListOfObjectsImpl : public IListOfObjects
{
public:
  ListOfObjectsImpl(size_t n) : m_ptrObjects(n)
  {
    std::generate(m_ptrObjects.begin(), m_ptrObjects.end(), []()
                  {
                    return std::unique_ptr<IObject>(new IObject());
                  });
  }

  size_t count() const override
  {
    return m_ptrObjects.size();
  }

  const IObject* get(size_t i) const override
  {
    return m_ptrObjects[i].get();
  }

  IObject* get(size_t i) override
  {
    return m_ptrObjects[i].get();
  }

private:
  std::vector<std::unique_ptr<IObject>> m_ptrObjects;
};

int main(int argc, char* argv[])
{
  ListOfObjectsImpl myList(100);

  std::for_each(myList.begin(), myList.end(), [](IListOfObjects::proxy pObject)
                {
                  pObject->m_value = 3.14159;
                });
}
