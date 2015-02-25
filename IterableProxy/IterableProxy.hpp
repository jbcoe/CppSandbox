template <typename Object_t, typename Get_f, typename Count_f>
class IterableProxy
{
public:
  IterableProxy(Get_f f_get, Count_f f_count)
      : m_f_get{f_get}, m_f_count{f_count}
  {
  }

  class iterator
  {
  public:
    iterator(Get_f& f_get, size_t index = 0) : m_index(index), m_f_get{f_get}
    {
    }

    const Object_t* operator*()
    {
      return m_f_get(m_index);
    }
    iterator& operator++()
    {
      ++m_index;
      return *this;
    }
    bool operator==(const iterator& i) const
    {
      return m_index == i.m_index;
    }
    bool operator!=(const iterator& i) const
    {
      return m_index != i.m_index;
    }

  private:
    size_t m_index;
    Get_f& m_f_get;
  };

  iterator begin()
  {
    return iterator(m_f_get);
  }

  iterator end()
  {
    return iterator(m_f_get, m_f_count());
  }

private:
  Get_f m_f_get;
  Count_f m_f_count;
};

template <typename Object_t, typename Get_f, typename Count_f>
IterableProxy<Object_t, Get_f, Count_f> make_iterable_proxy(Get_f f_get,
                                                            Count_f f_count)
{
  return IterableProxy<Object_t, Get_f, Count_f>(f_get, f_count);
}
