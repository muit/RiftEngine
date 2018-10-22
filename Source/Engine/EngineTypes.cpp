// Copyright 2015-2019 Piperift - All rights reserved

#include "EngineTypes.h"


#define DEFINE_REFLECTION_TYPE(Type) const Name ReflectionTypeTraits<Type>::name { #Type };

DEFINE_REFLECTION_TYPE(uint8);
DEFINE_REFLECTION_TYPE(int32);
DEFINE_REFLECTION_TYPE(Name);
DEFINE_REFLECTION_TYPE(String);
