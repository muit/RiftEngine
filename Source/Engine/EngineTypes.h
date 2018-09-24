// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "GL/gl3w.h"
#include "Platform/Platform.h"
#include "Util/String.h"
#include "Util/Name.h"


template<typename Type>
struct ReflectionTypeTraits {
	static constexpr bool valid = false;
};

#define DECLARE_REFLECTION_TYPE(Type)\
template <>\
struct ReflectionTypeTraits<Type> {\
static constexpr bool valid = true;\
static const Name name;\
};

/** Registry new editor-supported types here and on "EngineTypes.cpp" */
DECLARE_REFLECTION_TYPE(uint8);
DECLARE_REFLECTION_TYPE(int32);
DECLARE_REFLECTION_TYPE(Name);
DECLARE_REFLECTION_TYPE(String);

