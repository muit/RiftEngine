// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <string>
#include <vector>

#include "Property.h"
#include "Class.h"
#include "Macros.h"

// Wrap some types so we can use them at compile time
template<int N> struct MetaInt : MetaInt<N - 1> { static constexpr int value = N; };
template<> struct MetaInt<0> { static constexpr int value = 0; };


#define CLASS(type, parent) \
public:\
using Super = parent;\
ORPHAN_CLASS(type)

#define ORPHAN_CLASS(type) \
private:\
using __meta_type = type; \
friend Class<__meta_type>; \
\
public:\
static Class<__meta_type>* StaticClass() {\
	return Class<__meta_type>::GetStatic();\
}\
static constexpr MetaInt<0> __meta_Counter(MetaInt<0>);\
template<int N> static void __meta_RegistrySpecifier(MetaInt<N>) {}\
\
static inline void __meta_InitPropChain() {\
	__meta_RegistrySpecifier(MetaInt<0>{}); \
}\
private:


#define PROP(type, name, ...) __PROPERTY_IMPL(type, name, CAT(__meta_id_, name), __LINE__, __VA_ARGS__)

#define __PROPERTY_IMPL(type, name, id_name, line, ...) \
static constexpr int id_name = decltype(__meta_Counter(MetaInt<255>{}))::value; \
static constexpr MetaInt<id_name + 1> __meta_Counter(MetaInt<id_name + 1>); \
static void __meta_RegistrySpecifier(MetaInt<id_name>) { \
\
	StaticClass()->RegistryProperty<type>(#name, [](__meta_type& instance)\
	{\
		return &instance.name;\
	}, { __VA_ARGS__ }); \
\
	__meta_RegistrySpecifier(MetaInt<id_name + 1>{}); \
};
