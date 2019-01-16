// Copyright 2015-2019 Piperift - All rights reserved

#include "EntityManager.h"

#include "Core/Serialization/Archive.h"
#include "Gameplay/Components/CTransform.h"


bool EntityManager::Serialize(Archive& ar)
{
	bool bResult = Super::Serialize(ar);

	if (ar.IsSaving())
	{
		Archive* arPtr = &ar;
		registry.each([this, arPtr](EntityId entity) {
			SerializeEntity(*arPtr, entity);
		});
	}
	else
	{
		registry.reset();

	}

	return bResult;
}

void EntityManager::SerializeEntity(Archive& ar, const EntityId& entity)
{
	ar.BeginObject("components");
	{
		// #TODO: Generalize for all components
		if (registry.has<CEntity>(entity))
		{
			ar.BeginObject(CEntity::StaticStruct()->GetSName());

			CEntity& comp = registry.get<CEntity>(entity);
			comp.SerializeReflection(ar);
			ar.EndObject();
		}

		if (registry.has<CTransform>(entity))
		{
			ar.BeginObject(GetReflectableName<CTransform>().ToString());

			CTransform& transform = registry.get<CTransform>(entity);
			transform.SerializeReflection(ar);
			ar.EndObject();
		}
	}
	ar.EndObject();
}
