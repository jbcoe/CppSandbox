#pragma once

template <typename T>
struct BaseTypeTraits { 
	const static bool HasBaseType = false; 
};

template <bool hasBaseType, typename T>
struct BaseTypeImpl;

template <typename T>
struct BaseTypeImpl <true, T> { typedef typename T::BaseType Type; };

template <typename T>
struct BaseTypeImpl<false, T> { typedef T Type; };

template <typename T>
struct BaseType { 
	typedef typename BaseTypeImpl<BaseTypeTraits<T>::HasBaseType,T>::Type Type; 
};
