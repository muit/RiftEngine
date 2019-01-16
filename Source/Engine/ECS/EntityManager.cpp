// Copyright 2015-2019 Piperift - All rights reserved

#include "EntityManager.h"

#include "Core/Serialization/Archive.h"

#include "Gameplay/Components/CEntity.h"
#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CMesh.h"


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
		SerializeComponent<CEntity>(ar, entity);
		SerializeComponent<CTransform>(ar, entity);
		SerializeComponent<CMesh>(ar, entity);
	}
	ar.EndObject();
}
