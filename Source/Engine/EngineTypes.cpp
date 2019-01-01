// Copyright 2015-2019 Piperift - All rights reserved

#include "EngineTypes.h"


#define DEFINE_REFLECTION_TYPE(Type) const Name ReflectionTypeTraits<Type>::name { TX(#Type) };

DEFINE_REFLECTION_TYPE(uint8);
DEFINE_REFLECTION_TYPE(int32);
DEFINE_REFLECTION_TYPE(float);

DEFINE_REFLECTION_TYPE(Name);
DEFINE_REFLECTION_TYPE(String);
DEFINE_REFLECTION_TYPE(Guid);

DEFINE_REFLECTION_TYPE(v2);
DEFINE_REFLECTION_TYPE(v3);
