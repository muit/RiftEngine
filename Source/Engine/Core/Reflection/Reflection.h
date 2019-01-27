// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreEngine.h"

//#include "Property.h"
#include "Macros.h"
#include "ReflectionTags.h"
#include "Core/Platform/Platform.h"

/**
 * There are 3 types of reflected data structures:
 *
 * - POD: No inheritance, base classes or Garbage Collection. Only contains data.
 * - STRUCT: Inheritance but no logic or garbage collection. Used for Components.
 * - CLASS: Inheritance, Logic and Garbage Collection. Used as Objects.
 */


// Wrap some types so we can use them at compile time
template<u32 N> struct MetaCounter : MetaCounter<N - 1> { static constexpr u32 value = N; };
template<> struct MetaCounter<0> { static constexpr u32 value = 0; };


#define CLASS(type, parent, tags) \
public:\
using Super = parent;\
\
virtual inline void SerializeReflection(Archive& ar) override {\
	Super::SerializeReflection(ar);\
	__meta_SerializeProperty(ar, MetaCounter<0>{});\
}\
static void __meta_RegistryClass() {\
	StaticClass()->Registry<Super>(Name{ TX(#type) });\
BASECLASS(type, tags)

#define ORPHAN_CLASS(type, tags) \
public:\
virtual inline void SerializeReflection(Archive& ar) {\
	__meta_SerializeProperty(ar, MetaCounter<0>{});\
}\
static void __meta_RegistryClass() {\
	static_assert(eastl::is_convertible<__meta_type, BaseObject >::value, "Type does not inherit Object!");\
	StaticClass()->Registry(Name{ TX(#type) });\
BASECLASS(type, tags)

#define BASECLASS(type, inTags)\
	constexpr ReflectionTags tags = ReflectionTagsInitializer<inTags>::value;\
	static_assert(!(tags & DetailsEdit), "Classes can't use DetailsEdit"); \
	static_assert(!(tags & DetailsView), "Classes can't use DetailsView"); \
	StaticClass()->RegistryTags(tags);\
}\
private:\
using __meta_type = type;\
friend TClass<__meta_type>;\
\
private:\
static inline TClass<__meta_type>* StaticType() {\
	return StaticClass();\
}\
public:\
static TClass<__meta_type>* StaticClass() {\
	return TClass<__meta_type>::GetStatic();\
}\
\
static inline void __meta_RegistryProperties() {\
	__meta_RegistryProperty(MetaCounter<0>{});\
}\
\
private:\
static constexpr MetaCounter<0> __meta_Counter(MetaCounter<0>);\
template<u32 N> static void __meta_RegistryProperty(MetaCounter<N>) {}\
template<u32 N> FORCEINLINE void __meta_SerializeProperty(Archive&, MetaCounter<N>) {}



#define STRUCT(type, parent, tags) \
public:\
using Super = parent;\
\
virtual StructType* GetStruct() const override { return StaticStruct(); }\
virtual inline void SerializeReflection(Archive& ar) override {\
	Super::SerializeReflection(ar);\
	__meta_SerializeProperty(ar, MetaCounter<0>{});\
}\
static void __meta_RegistryStruct() {\
	static_assert(eastl::is_convertible<__meta_type, Struct >::value, "Type does not inherit Struct!");\
	StaticStruct()->Registry<Super>(Name{ TX(#type) });\
BASESTRUCT(type, tags)

#define ORPHAN_STRUCT(type, tags) \
public:\
virtual inline void SerializeReflection(Archive& ar) {\
	__meta_SerializeProperty(ar, MetaCounter<0>{});\
}\
static void __meta_RegistryStruct() {\
	StaticStruct()->Registry(Name{ TX(#type) });\
BASESTRUCT(type, tags)

#define BASESTRUCT(type, inTags)\
	constexpr ReflectionTags tags = ReflectionTagsInitializer<inTags>::value;\
	static_assert(!(tags & DetailsEdit), "Structs can't use DetailsEdit"); \
	static_assert(!(tags & DetailsView), "Structs can't use DetailsView"); \
	StaticStruct()->RegistryTags(tags);\
}\
private:\
using __meta_type = type;\
friend TStruct<__meta_type>;\
\
static constexpr MetaCounter<0> __meta_Counter(MetaCounter<0>);\
template<u32 N> static void __meta_RegistryProperty(MetaCounter<N>) {}\
template<u32 N> FORCEINLINE void __meta_SerializeProperty(Archive&, MetaCounter<N>) {}\
\
static inline TStruct<__meta_type>* StaticType() {\
	return StaticStruct();\
}\
public:\
static TStruct<__meta_type>* StaticStruct() {\
	return TStruct<__meta_type>::GetStatic();\
}\
\
static inline void __meta_RegistryProperties() {\
	__meta_RegistryProperty(MetaCounter<0>{});\
}



#define PROP(type, name, tags) __PROPERTY_IMPL(type, name, CAT(__meta_id_, name), tags)

#define __PROPERTY_IMPL(type, name, id_name, inTags)\
static constexpr u32 id_name = decltype(__meta_Counter(MetaCounter<255>{}))::value;\
static constexpr MetaCounter<id_name + 1> __meta_Counter(MetaCounter<id_name + 1>);\
\
static void __meta_RegistryProperty(MetaCounter<id_name>) {\
	static_assert(IsReflectableType<type>(), "Only reflectable types are allowed (u8, u32, i32, Name, String...)");\
\
	constexpr ReflectionTags tags = ReflectionTagsInitializer<inTags>::value;\
	static_assert(!(tags & Abstract), "Properties can't be Abstract");\
\
	StaticType()->RegistryProperty<type>(TX(#name), [](BaseObject* baseInstance)\
	{\
		if(__meta_type* instance = dynamic_cast<__meta_type*>(baseInstance))\
			return &instance->name;\
		return (type*)nullptr;\
	}, tags);\
\
	/* Registry next property if any */\
	__meta_RegistryProperty(MetaCounter<id_name + 1>{});\
};\
\
FORCEINLINE void __meta_SerializeProperty(Archive& ar, MetaCounter<id_name>) {\
\
	constexpr ReflectionTags tags = ReflectionTagsInitializer<inTags>::value;\
	\
	if constexpr(!(tags & Transient))\
	{/* Don't serialize property if Transient */\
		ar(#name , name);\
	}\
	/* Serialize next property if any */\
	__meta_SerializeProperty(ar, MetaCounter<id_name + 1>{});\
};
