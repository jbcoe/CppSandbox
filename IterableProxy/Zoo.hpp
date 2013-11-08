#include <memory>
#include <vector>

class IAnimal
{
public:
  virtual ~IAnimal()
  {
  }
  virtual const char* makeNoise() const = 0;
};

class Tiger : public IAnimal
{
public:
  const char* makeNoise() const
  {
    return "Purr";
  }
};
class Bear : public IAnimal
{
public:
  const char* makeNoise() const
  {
    return "Growl";
  }
};
class Lion : public IAnimal
{
public:
  const char* makeNoise() const
  {
    return "Roar";
  }
};

class IZoo
{
public:
  virtual ~IZoo()
  {
  }

  virtual size_t BearCount() const = 0;
  virtual const Bear* GetBear(size_t index) const = 0;

  virtual size_t TigerCount() const = 0;
  virtual const Tiger* GetTiger(size_t index) const = 0;

  virtual size_t LionCount() const = 0;
  virtual const Lion* GetLion(size_t index) const = 0;
};

class Zoo : public IZoo
{
public:
  Zoo()
  {
    std::generate_n(std::back_inserter(m_lions), 10, []()
    { return std::unique_ptr<Lion>(new Lion()); });
  }

  size_t BearCount() const
  {
    return m_bears.size();
  }
  const Bear* GetBear(size_t index) const
  {
    return m_bears[index].get();
  }

  size_t TigerCount() const
  {
    return m_tigers.size();
  }
  const Tiger* GetTiger(size_t index) const
  {
    return m_tigers[index].get();
  }

  size_t LionCount() const
  {
    return m_lions.size();
  }
  const Lion* GetLion(size_t index) const
  {
    return m_lions[index].get();
  }

private:
  std::vector<std::unique_ptr<Lion>> m_lions;
  std::vector<std::unique_ptr<Tiger>> m_tigers;
  std::vector<std::unique_ptr<Bear>> m_bears;
};
