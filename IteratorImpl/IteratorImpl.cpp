#include <memory>
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

class IObject
{
public:
  virtual const char* Name() const = 0;
};

template <class T>
class TObjectSet
{
public:
  typedef T Object_t;

protected:
  class IteratorImpl
  {
  public:
    virtual IteratorImpl& operator++() = 0;
    virtual bool operator==(const IteratorImpl& it) const = 0;
    virtual bool operator!=(const IteratorImpl& it) const = 0;
    virtual T* operator*() = 0;
    virtual std::unique_ptr<IteratorImpl> Clone() const = 0;
    virtual ~IteratorImpl() {}
  };

public:
  class iterator
  {
  public:
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::forward_iterator_tag iterator_category;

    iterator(std::unique_ptr<IteratorImpl>&& ptrImpl)
        : itImpl(std::move(ptrImpl))
    {
    }

    iterator(const iterator& it) : itImpl(std::move(it.itImpl->Clone())) {}

    const iterator& operator=(const iterator& it)
    {
      itImpl = (std::move(it.m_itImpl->Clone()));
      return *this;
    }

    iterator& operator++()
    {
      itImpl->operator++();
      return *this;
    };

    bool operator==(const iterator& it) const
    {
      return itImpl->operator==(*it.itImpl);
    };

    bool operator!=(const iterator& it) const
    {
      return itImpl->operator!=(*it.itImpl);
    };

    T* operator*() { return itImpl->operator*(); };

  private:
    std::unique_ptr<IteratorImpl> itImpl;
  };

  virtual iterator begin() = 0;
  virtual iterator end() = 0;
};

class IObjectSet : public TObjectSet<IObject>
{
};

class CObjectImpl : public IObject
{
public:
  CObjectImpl(const char* name) : m_name(name) {}
  virtual const char* Name() const { return m_name.c_str(); }

private:
  std::string m_name;
};

template <class ObjImpl_t, class SetInterface_t>
class TObjectSetImpl : public SetInterface_t
{
private:
  class concrete_iterator : public IObjectSet::IteratorImpl
  {
  public:
    concrete_iterator(std::vector<std::unique_ptr<CObjectImpl>>::iterator it)
        : m_it(it)
    {
    }

    virtual IteratorImpl& operator++()
    {
      m_it++;
      return *this;
    }

    virtual std::unique_ptr<IteratorImpl> Clone() const
    {
      std::unique_ptr<IteratorImpl> ptr(new concrete_iterator(m_it));
      return std::move(ptr);
    }

    virtual bool operator==(const IteratorImpl& it) const
    {
      return static_cast<const concrete_iterator&>(it).m_it == m_it;
    }

    virtual bool operator!=(const IteratorImpl& it) const
    {
      return static_cast<const concrete_iterator&>(it).m_it != m_it;
    }

    virtual IObjectSet::Object_t* operator*() { return (*m_it).get(); }

  private:
    std::vector<std::unique_ptr<CObjectImpl>>::iterator m_it;
  };

public:
  typename IObjectSet::iterator begin()
  {
    return IObjectSet::iterator(std::unique_ptr<concrete_iterator>(
        new concrete_iterator(m_vObjects.begin())));
  }

  typename IObjectSet::iterator end()
  {
    return IObjectSet::iterator(std::unique_ptr<concrete_iterator>(
        new concrete_iterator(m_vObjects.end())));
  }

  template <typename... Args>
  void Add(Args... args)
  {
    m_vObjects.push_back(std::unique_ptr<ObjImpl_t>(new ObjImpl_t(args...)));
  }

private:
  std::vector<std::unique_ptr<ObjImpl_t>> m_vObjects;
};

class CObjectSetImpl : public TObjectSetImpl<CObjectImpl, IObjectSet>
{
};


int main(int argc, char* argv[])
{
  CObjectSetImpl mySet;
  mySet.Add("Huey");
  mySet.Add("Duey");
  mySet.Add("Louie");
  mySet.Add("Donald");
  mySet.Add("Mickey");
  mySet.Add("Minnie");

  IObjectSet& setInterface = mySet;

  for (const IObject* pObj : setInterface)
  {
    static int i = 0;
    std::cout << "Encountered object " << i++ << " " << pObj->Name()
              << std::endl;
  };
}
