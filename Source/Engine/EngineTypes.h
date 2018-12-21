// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EASTL/utility.h"

#include "Core/Platform/Platform.h"
#include "Core/Containers/Array.h"
#include "Core/Strings/String.h"
#include "Core/Strings/Name.h"


template <typename T1, typename T2>
using Pair = eastl::pair<T1, T2>;


template<typename Type>
struct ReflectionTypeTraits {
	static constexpr bool valid = false;
};

#define DECLARE_REFLECTION_TYPE(Type)\
template <>\
struct ReflectionTypeTraits<Type> {\
static constexpr bool valid = true;\
static const Name name;\
}

/** Registry new editor-supported types here and on "EngineTypes.cpp" */
DECLARE_REFLECTION_TYPE(uint8);
DECLARE_REFLECTION_TYPE(int32);
DECLARE_REFLECTION_TYPE(float);
DECLARE_REFLECTION_TYPE(Name);
DECLARE_REFLECTION_TYPE(String);
