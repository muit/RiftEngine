// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EASTL/utility.h"

#include "Core/TypeTraits.h"
#include "Core/Platform/Platform.h"
#include "Core/Containers/Array.h"
#include "Core/Strings/String.h"
#include "Core/Strings/Name.h"
#include "Core/Misc/Guid.h"


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
DECLARE_REFLECTION_TYPE(Guid);


template <typename T1, typename T2>
using Pair = eastl::pair<T1, T2>;