// Copyright 2015-2019 Piperift - All rights reserved

#include "EntityId.h"
#include "Core/Log.h"
#include "Core/World.h"
#include "Core/Serialization/Archive.h"


bool Serialize(Archive& ar, const char* name, EntityId& val)
{
	if (!ar.context)
	{
		Log::Warning("Tried to serialize EntityId '%s' without world context.", name);
		return false;
	}

	if (ar.IsLoading())
	{
		// Find EntityId on the cache map
		Guid guid {};
		ar(name, guid);
		if (guid.IsValid())
		{
			auto ecs = ar.context->GetECS();

			const auto& cache = ecs->GetGuidCache();
			const auto it = cache.find(guid);

			val = (it != cache.end()) ? it->second : NoEntity;
		}
		else
		{
			val = NoEntity;
		}
	}
	else
	{
		// Get Guid from the component
		auto ecs = ar.context->GetECS();
		if (ecs->IsValid(val))
		{
			// Save CEntity guid
			ar(name, ecs->Get<CEntity>(val).id);
		}
	}
	return true;
}
