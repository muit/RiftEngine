// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EASTL/type_traits.h>


/** CLASS TRAITS */

/** Default traits go here */
template <typename T>
struct BaseClassTraits { enum {
	HasCustomSerialize = false
};};

/** Custom traits go here */
#define DEFINE_CLASS_TRAITS(Class, traits)\
template <>\
struct ClassTraits<Class> : public BaseClassTraits<Class> {\
	enum traits\
;}

template <typename T>
struct ClassTraits : public BaseClassTraits<T> {};


/** SIZE SELECTORS */

template < class T, size_t size >
struct IsSmallerType : eastl::integral_constant< bool, (sizeof(T) <= size) > {};

template < class T, size_t size >
struct IsBiggerType : eastl::integral_constant< bool, (sizeof(T) > size) > {};


#define EnableIfSmallerType(size)    class = eastl::enable_if< IsSmallerType< T, size >::value >
#define EnableIfNotSmallerType(size) class = eastl::enable_if< !IsSmallerType< T, size >::value >

#define EnableIfBiggerType(size)    class = eastl::enable_if< IsBiggerType< T, size >::value >
#define EnableIfNotBiggerType(size) class = eastl::enable_if< !IsBiggerType< T, size >::value >

#define EnableIfPassByValue(T)    class = eastl::enable_if<  IsSmallerType< T, sizeof(size_t) >::value && eastl::is_copy_constructible< T >::type>
#define EnableIfNotPassByValue(T) class = eastl::enable_if<!(IsSmallerType< T, sizeof(size_t) >::value && eastl::is_copy_constructible< T >::type)>

#define EnableIfAll    class = void


template<typename Type>
struct ReflectionTypeTraits {
	static constexpr bool valid = false;
};
