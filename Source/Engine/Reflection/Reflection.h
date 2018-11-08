// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <string>
#include <vector>

//#include "Property.h"
#include "Macros.h"
#include "ReflectionTags.h"


// Wrap some types so we can use them at compile time
template<int N> struct MetaInt : MetaInt<N - 1> { static constexpr int value = N; };
template<> struct MetaInt<0> { static constexpr int value = 0; };


#define CLASS(type, parent, tags) \
public:\
using Super = parent;\
\
virtual inline void SerializeReflection(Archive& ar) override {\
	Super::SerializeReflection(ar);\
	__meta_SerializeProperty(ar, MetaInt<0>{});\
}\
static void __meta_RegistryClass() {\
	StaticClass()->RegistryClass<Super>(#type);\
BASECLASS(type, tags)

#define ORPHAN_CLASS(type, tags) \
public:\
virtual inline void SerializeReflection(Archive& ar) {\
	__meta_SerializeProperty(ar, MetaInt<0>{});\
}\
static void __meta_RegistryClass() {\
	StaticClass()->RegistryClass(#type);\
BASECLASS(type, tags)

#define BASECLASS(type, inTags)\
	constexpr ReflectionTags tags = ReflectionTagsInitializer<inTags>::value;\
	static_assert(!(tags & DetailsEdit), "Classes can't use DetailsEdit"); \
	static_assert(!(tags & DetailsView), "Classes can't use DetailsView"); \
	StaticClass()->RegistryClassTags(tags);\
}\
private:\
using __meta_type = type;\
friend TClass<__meta_type>;\
\
public:\
static TClass<__meta_type>* StaticClass() {\
	return TClass<__meta_type>::GetStatic();\
}\
\
static inline void __meta_RegistryProperties() {\
	__meta_RegistryProperty(MetaInt<0>{});\
}\
\
private:\
static constexpr MetaInt<0> __meta_Counter(MetaInt<0>);\
template<int N> static void __meta_RegistryProperty(MetaInt<N>) {}\
template<int N> FORCEINLINE void __meta_SerializeProperty(Archive&, MetaInt<N>) {}


#define PROP(type, name, tags) __PROPERTY_IMPL(type, name, CAT(__meta_id_, name), tags)

#define __PROPERTY_IMPL(type, name, id_name, inTags)\
static constexpr int id_name = decltype(__meta_Counter(MetaInt<255>{}))::value;\
static constexpr MetaInt<id_name + 1> __meta_Counter(MetaInt<id_name + 1>);\
static void __meta_RegistryProperty(MetaInt<id_name>) {\
	static_assert(ReflectionTypeTraits<type>::valid, "Only reflectable types are allowed (uint8, int32, Name or String)");\
\
	constexpr ReflectionTags tags = ReflectionTagsInitializer<inTags>::value;\
	static_assert(!(tags & Abstract), "Properties can't be Abstract");\
\
	StaticClass()->RegistryProperty<type>(#name, [](BaseObject* baseInstance)\
	{\
		if(__meta_type* instance = dynamic_cast<__meta_type*>(baseInstance))\
			return &instance->name;\
		return (type*)nullptr;\
	}, tags);\
\
	/* Registry next property if any */\
	__meta_RegistryProperty(MetaInt<id_name + 1>{});\
};\
\
FORCEINLINE void __meta_SerializeProperty(Archive& ar, MetaInt<id_name>) {\
\
	constexpr ReflectionTags tags = ReflectionTagsInitializer<inTags>::value;\
	\
	if(!(tags & Transient))\
	{/* If prop is not transient (Compile time) */\
		ar(#name , name);\
	}\
	/* Serialize next property if any */\
	__meta_SerializeProperty(ar, MetaInt<id_name + 1>{});\
};
