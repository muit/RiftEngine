// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include <EASTL/numeric_limits.h>

#include "Core/Platform/Platform.h"
#include "EngineTypes.h"

using EntityId = u32;

constexpr EntityId NoEntity { eastl::numeric_limits<EntityId>::max() };

DEFINE_CLASS_TRAITS(EntityId,
	HasGlobalSerialize = true
);

bool Serialize(class Archive& ar, const char* name, EntityId& val);
