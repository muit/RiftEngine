// Copyright 2015-2019 Piperift - All rights reserved

#include "EntityManager.h"

#include "Serialization/Archive.h"


bool EntityManager::Serialize(Archive& ar)
{
	bool bResult = Super::Serialize(ar);

	if (ar.IsSaving())
	{
		/*registry.each([this, ar](EntityId entity) {
			SerializeEntity(ar, entity);
		});*/
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

	}
	ar.EndObject();
}
