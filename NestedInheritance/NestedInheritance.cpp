#include <iostream>
#include "NestedInheritanceTraits.h"
#include "BaseTypeTraits.h"

struct DataForBase
{
  int m_base;
};

struct DataForA
{
  int m_a;
};

struct DataForB
{
  int m_b;
};

class Interface
{
public:
  virtual ~Interface() {}
  virtual void DoBase() const = 0;
  virtual void DoA() const = 0;
  virtual void DoB() const = 0;
};

template <class TInterface>
class VirtualWrapper_A : public virtual TInterface
{
private:
  DataForA m_aData;

protected:
  ~VirtualWrapper_A() override {}

public:
  void DoA() const override {}

  typedef typename BaseType<TInterface>::Type BaseType;
};

template <typename T>
struct BaseTypeTraits<VirtualWrapper_A<T>>
{
  static const bool HasBaseType = true;
};


template <class TInterface>
class VirtualWrapper_B : public virtual TInterface
{
private:
  DataForB m_bData;

protected:
  ~VirtualWrapper_B() override {}

public:
  void DoB() const override {}

  typedef typename BaseType<TInterface>::Type BaseType;
};

template <typename T>
struct BaseTypeTraits<VirtualWrapper_B<T>>
{
  static const bool HasBaseType = true;
};


template <class TInterface>
class Wrapper_A : public TInterface
{
private:
  DataForA m_aData;

protected:
  ~Wrapper_A() override {}

public:
  void DoA() const override {}

  typedef typename BaseType<TInterface>::Type BaseType;
  typedef TInterface NestedType;
};

template <typename T>
struct BaseTypeTraits<Wrapper_A<T>>
{
  static const bool HasBaseType = true;
};

template <typename T>
struct NestedInheritanceTraits<Wrapper_A<T>>
{
  const static bool HasNestedInheritance = true;
};


template <class TInterface>
class Wrapper_B : public TInterface
{
private:
  DataForB m_bData;

protected:
  ~Wrapper_B() override {}

public:
  void DoB() const override {}

  typedef typename BaseType<TInterface>::Type BaseType;
  typedef TInterface NestedType;
};

template <typename T>
struct BaseTypeTraits<Wrapper_B<T>>
{
  static const bool HasBaseType = true;
};

template <typename T>
struct NestedInheritanceTraits<Wrapper_B<T>>
{
  const static bool HasNestedInheritance = true;
};


class VirtualDerived : public VirtualWrapper_A<Interface>,
                       public VirtualWrapper_B<Interface>
{
private:
  DataForBase m_baseData;

public:
  void DoBase() const override{};

  typedef Interface BaseType;
};

template <>
struct BaseTypeTraits<VirtualDerived>
{
  static const bool HasBaseType = true;
};


class NestedDerived : public Wrapper_A<Wrapper_B<Interface>>
{
private:
  DataForBase m_baseData;

public:
  void DoBase() const override{};

  typedef Wrapper_A<Wrapper_B<Interface>> NestedType;
};

template <>
struct NestedInheritanceTraits<NestedDerived>
{
  const static bool HasNestedInheritance = true;
};

template <>
struct BaseTypeTraits<NestedDerived>
{
  static const bool HasBaseType = true;
};


class DelegateA
{
  DataForA m_base;

public:
  virtual void DoA() const {}
};

class DelegateB
{
  DataForB m_base;

public:
  virtual void DoB() const {}
};

class DelegatingImpl : public Interface
{
private:
  DelegateA m_dA;
  DelegateB m_dB;

public:
  void DoBase() const override {}
  void DoA() const override { m_dA.DoA(); }
  void DoB() const override { m_dB.DoB(); }
};

class RawImpl : public Interface
{
private:
  DataForBase m_baseData;
  DataForA m_aData;
  DataForB m_bData;

public:
  void DoBase() const override {}
  void DoA() const override {}
  void DoB() const override {}
};


template <typename T>
struct TypeName;

#define DEFINE_NAME_FOR_TYPE(X)                                                \
  template <>                                                                  \
  struct TypeName<X>                                                           \
  {                                                                            \
    static const char* Name() { return #X; }                                   \
  };

DEFINE_NAME_FOR_TYPE(void*);
DEFINE_NAME_FOR_TYPE(int);
DEFINE_NAME_FOR_TYPE(Interface);
DEFINE_NAME_FOR_TYPE(DataForBase);
DEFINE_NAME_FOR_TYPE(DataForA);
DEFINE_NAME_FOR_TYPE(DataForB);
DEFINE_NAME_FOR_TYPE(RawImpl);
DEFINE_NAME_FOR_TYPE(VirtualDerived);
DEFINE_NAME_FOR_TYPE(NestedDerived);
DEFINE_NAME_FOR_TYPE(DelegatingImpl);

#undef DEFINE_NAME_FOR_TYPE

int main(int argc, char* argv[])
{
  VirtualDerived vd;
  vd.DoBase();
  vd.DoA();
  vd.DoB();

  NestedDerived nd;
  nd.DoBase();
  nd.DoA();
  nd.DoB();

  DelegatingImpl di;
  di.DoBase();
  di.DoA();
  di.DoB();

  RawImpl ri;
  ri.DoBase();
  ri.DoA();
  ri.DoB();

#define PRINT_SIZE_OF(thing)                                                   \
  std::cout << #thing                                                          \
            << " base type: " << TypeName<BaseType<thing>::Type>::Name()       \
            << ", "                                                            \
            << "nested inheritance depth "                                     \
            << NestedInheritance<thing>::NestDepth() << ", "                   \
            << "size: " << sizeof(thing) << std::endl

  PRINT_SIZE_OF(void*);
  PRINT_SIZE_OF(int);
  PRINT_SIZE_OF(Interface);
  PRINT_SIZE_OF(DataForBase);
  PRINT_SIZE_OF(DataForA);
  PRINT_SIZE_OF(DataForB);
  PRINT_SIZE_OF(RawImpl);
  PRINT_SIZE_OF(Wrapper_A<Interface>);
  PRINT_SIZE_OF(Wrapper_B<Interface>);
  PRINT_SIZE_OF(VirtualDerived);
  PRINT_SIZE_OF(NestedDerived);
  PRINT_SIZE_OF(DelegatingImpl);
  PRINT_SIZE_OF(Wrapper_A<Interface>);
  PRINT_SIZE_OF(Wrapper_A<Wrapper_A<Interface>>);
  PRINT_SIZE_OF(Wrapper_A<Wrapper_A<Wrapper_A<Interface>>>);
  PRINT_SIZE_OF(Wrapper_A<Wrapper_A<Wrapper_A<Wrapper_A<Interface>>>>);

#undef PRINT_SIZE_OF
}
