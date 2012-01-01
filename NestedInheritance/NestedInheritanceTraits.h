#pragma once

template <typename T>
struct NestedInheritanceTraits {
	const static bool HasNestedInheritance = false;
};

template <bool hasNestedInheritance, int nestDepth, typename T>
struct NestedInheritanceImpl;

template <int nestDepth, typename T>
struct NestedInheritanceImpl <true, nestDepth, T>
{
	static constexpr int NestDepth() { 
		return NestedInheritanceImpl<
			NestedInheritanceTraits<typename T::NestedType>::HasNestedInheritance, 
			nestDepth + 1, 
			typename T::NestedType
		>::NestDepth();
	}
};

template <int nestDepth, typename T>
struct NestedInheritanceImpl <false, nestDepth, T>
{
 	static constexpr int NestDepth() { return nestDepth; }
};

template <typename T>
struct NestedInheritance
{
	static constexpr int NestDepth() 
	{ 
		return NestedInheritanceImpl<NestedInheritanceTraits<T>::HasNestedInheritance,0,T>::NestDepth(); 
	}
};
