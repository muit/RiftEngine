// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EASTL/type_traits.h>


/** CLASS TRAITS */

/** Default traits go here */
template <typename T>
struct BaseClassTraits { enum {
	HasCustomSerialize = false,
	HasDetailsWidget = false,
	HasPostSerialize = false
};};

/** Custom traits go here */
#define DEFINE_CLASS_TRAITS(Class, ...)\
template <>\
struct ClassTraits<Class> : public BaseClassTraits<Class> {\
	enum {__VA_ARGS__};\
}
#define DEFINE_TEMPLATE_CLASS_TRAITS(Class, ...)\
template <typename T>\
struct ClassTraits<Class<T>> : public BaseClassTraits<Class<T>> {\
	enum {__VA_ARGS__};\
}

template <typename T>
struct ClassTraits : public BaseClassTraits<T> {};


template<typename T, typename = void>
struct IsDefined {
	static constexpr bool value = false;
};

template<typename T>
struct IsDefined<T, decltype(typeid(T), void())> {
	static constexpr bool value = true;
};


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


template<typename T>
struct HasItemType
{
private:
	template<typename V> static void impl(decltype(typename V::ItemType(), int()));
	template<typename V> static bool impl(char);

public:
	static const bool value = eastl::is_void<decltype(impl<T>(0))>::value;
};

template<typename T>
inline constexpr bool IsArrayType() {
	// Check if we are dealing with a TArray
	if constexpr (HasItemType<T>::value)
		return eastl::is_same<TArray<typename T::ItemType>, T>::value;

	return false;
}

template<typename T>
inline constexpr bool IsAssetType() {
	// Check if we are dealing with a TArray
	if constexpr (HasItemType<T>::value)
		return eastl::is_same<TAssetPtr<typename T::ItemType>, T>::value;

	return false;
}

template<typename T>
inline constexpr bool IsReflectableType() {
	if constexpr (IsArrayType<T>())
	{
		return IsReflectableType<typename T::ItemType>();
	}
	return IsAssetType<T>();
}

template <bool B, class T = void>
using EnableIf = eastl::enable_if<B, T>;

template <bool B, class T = void>
using DisableIf = eastl::disable_if<B, T>;


template <bool B, class T = void>
using EnableIfT = eastl::enable_if_t<B, T>;

template <bool B, class T = void>
using DisableIfT = eastl::disable_if_t<B, T>;

