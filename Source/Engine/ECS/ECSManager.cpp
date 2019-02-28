// Copyright 2015-2019 Piperift - All rights reserved

#include "ECSManager.h"

#include "Core/Serialization/Archive.h"

#include "Gameplay/Components/CEntity.h"
#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CMesh.h"
#include "Gameplay/Components/CCamera.h"
#include "Gameplay/Components/CEditorCamera.h"
#include "Gameplay/Components/CPointLight.h"
#include "Gameplay/Components/CDirectionalLight.h"
#include "Gameplay/Systems/SEditorCamera.h"
#include "Gameplay/Systems/SRenderMeshes.h"
#include "Gameplay/Systems/SRenderCamera.h"
#include "Gameplay/Systems/SLighting.h"


bool ECSManager::Serialize(Archive& ar)
{
	bool bResult = Super::Serialize(ar);

	ar.BeginObject("entities");
	if (ar.IsLoading())
	{
		registry.reset();

		u32 size;
		ar.SerializeArraySize(size);

		registry.reserve(size);

		for (u32 i = 0; i < size; ++i)
		{
			ar.BeginObject(i);
			if (ar.IsObjectValid())
			{
				EntityId entity = CreateEntity("");
				SerializeEntity(ar, entity);
			}
			ar.EndObject();
		}
	}
	else
	{
		u32 size = (u32)registry.alive();
		ar.SerializeArraySize(size);

		u32 i = 0;
		Archive* arPtr = &ar;
		registry.each([this, arPtr, i](EntityId entity) mutable
		{
			CEntity& entityComp = registry.get<CEntity>(entity);
			if (!entityComp.bTransient)
			{
				arPtr->BeginObject(i);
				{
					SerializeEntity(*arPtr, entity);
				}
				arPtr->EndObject();
				++i;
			}
		});
	}
	ar.EndObject();

	return bResult;
}

void ECSManager::SerializeEntity(Archive& ar, EntityId entity)
{
	SerializeComponent<CEntity>(ar, entity);
	SerializeComponent<CTransform>(ar, entity);
	SerializeComponent<CMesh>(ar, entity);
	SerializeComponent<CEditorCamera>(ar, entity);
	SerializeComponent<CPointLight>(ar, entity);
	SerializeComponent<CDirectionalLight>(ar, entity);
}

void ECSManager::RegistrySystems()
{
	// #TODO: Externalize system registry
	RegistrySystem<SEditorCamera>();

	// Rendering
	RegistrySystem<SRenderCamera>();
	RegistrySystem<SLighting>();
	RegistrySystem<SRenderMeshes>();
}

void ECSManager::RegistrySingletons()
{
}
