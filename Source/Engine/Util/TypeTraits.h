// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EASTL/type_traits.h"


template < class T, size_t size >
struct IsSmallerType : eastl::integral_constant< bool, (sizeof(T) <= size) > {};

template < class T, size_t size >
struct IsBiggerType : eastl::integral_constant< bool, (sizeof(T) > size) > {};


#define EnableIfSmallerType(size)    typename eastl::enable_if< IsSmallerType< T, size >::value >::type* = nullptr
#define EnableIfNotSmallerType(size) typename eastl::enable_if< !IsSmallerType< T, size >::value >::type* = nullptr

#define EnableIfBiggerType(size)    typename eastl::enable_if< IsBiggerType< T, size >::value >::type* = nullptr
#define EnableIfNotBiggerType(size) typename eastl::enable_if< !IsBiggerType< T, size >::value >::type* = nullptr

#define EnableIfPassByValue    typename eastl::enable_if<   IsSmallerType< T, sizeof(size_t) >::value && eastl::is_copy_constructible< T >::value >::type* = nullptr
#define EnableIfNotPassByValue typename eastl::enable_if< !(IsSmallerType< T, sizeof(size_t) >::value && eastl::is_copy_constructible< T >::value) >::type* = nullptr
