// Copyright 2015-2019 Piperift - All rights reserved

#include "EntityId.h"
#include "World.h"
#include "Core/Log.h"
#include "Core/Serialization/Archive.h"


bool EntityId::Serialize(class Archive& ar, const char* name)
{
	if (!ar.context)
	{
		Log::Warning("Tried to serialize EntityId '%s' without world context.", name);
		return false;
	}

	if (ar.IsLoading())
	{
		// Find EntityId on the cache map
		Guid guid{};
		ar(name, guid);
		if (guid.IsValid())
		{
			auto ecs = ar.context->GetECS();

			const auto& cache = ecs->GetGuidCache();
			const auto* idPtr = cache.Find(guid);
			id = idPtr? *idPtr : NoEntity;
		}
		else
		{
			id = NoEntity;
		}
	}
	else
	{
		// Get Guid from the component
		auto ecs = ar.context->GetECS();
		if (ecs->IsValid(*this))
		{
			// Save CEntity guid
			ar(name, ecs->Get<CEntity>(*this).id);
		}
	}
	return true;
}
