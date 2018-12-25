// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EASTL/type_traits.h>


template<typename Type>
struct ReflectionTypeTraits {
	static constexpr bool valid = false;
};



template <typename T>
struct BaseClassTraits { enum {
	HasCustomSerialize = false
};};

template <typename T>
struct ClassTraits : public BaseClassTraits<T> {};

#define DEFINE_CLASS_TRAITS(Class, traits)\
template <>\
struct ClassTraits<Class> : public BaseClassTraits<Class> {\
	enum traits\
;}


template < class T, size_t size >
struct IsSmallerType : eastl::integral_constant< bool, (sizeof(T) <= size) > {};

template < class T, size_t size >
struct IsBiggerType : eastl::integral_constant< bool, (sizeof(T) > size) > {};


#define EnableIfSmallerType(size)    class = eastl::enable_if< IsSmallerType< T, size >::value >
#define EnableIfNotSmallerType(size) class = eastl::enable_if< !IsSmallerType< T, size >::value >

#define EnableIfBiggerType(size)    class = eastl::enable_if< IsBiggerType< T, size >::value >
#define EnableIfNotBiggerType(size) class = eastl::enable_if< !IsBiggerType< T, size >::value >

#define EnableIfPassByValue    class = eastl::enable_if<  IsSmallerType< T, sizeof(size_t) >::value && eastl::is_copy_constructible< T >::value>
#define EnableIfNotPassByValue class = eastl::enable_if<!(IsSmallerType< T, sizeof(size_t) >::value && eastl::is_copy_constructible< T >::value)>

#define EnableIfAll    class = void
