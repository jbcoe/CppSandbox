#pragma once

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// If a class specializes BaseTypeTraits with
// HasBaseType = true
// it must define a typedef as follows:
// typedef typename BaseType<SUPER_CLASS_TYPE>::Type BaseType;
//
// This typedef is used to work out the base class: the class at
// the heart of a series of templated inheritance wrappers.
//
// For instance where Wrapper is a template class where Wrapper inherits
// from the template type T:
//
// Wrapper<Wrapper<Wrapper<int>>>
//
// has base type int
//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

template <typename T>
struct BaseTypeTraits
{
  const static bool HasBaseType = false;
};

template <bool hasBaseType, typename T>
struct BaseTypeImpl;

template <typename T>
struct BaseTypeImpl<true, T>
{
  typedef typename T::BaseType Type;
};

template <typename T>
struct BaseTypeImpl<false, T>
{
  typedef T Type;
};

template <typename T>
struct BaseType
{
  typedef typename BaseTypeImpl<BaseTypeTraits<T>::HasBaseType, T>::Type Type;
};
