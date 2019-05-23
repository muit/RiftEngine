// Copyright 2015-2019 Piperift - All rights reserved

#include "ECSManager.h"

#include "World.h"
#include "Core/Serialization/Archive.h"

#include "Gameplay/Components/CEntity.h"
#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CMesh.h"
#include "Gameplay/Components/CCamera.h"
#include "Gameplay/Components/CEditorCamera.h"
#include "Gameplay/Components/CPointLight.h"
#include "Gameplay/Components/CDirectionalLight.h"
#include "Gameplay/Systems/SLighting.h"

#include "Physics/2D/Components/CBody2D.h"
#include "Physics/2D/Components/CCollider2D.h"
#include "Physics/2D/Components/CBoxCollider2D.h"
#include "Physics/2D/Components/CCircleCollider2D.h"
#include "Physics/3D/Components/CBody.h"

#include "Gameplay/Game/Components/CPlayer.h"
#include "Gameplay/Game/Components/CPlatform.h"
#include "Gameplay/Game/Systems/SPlayer.h"
#include "Gameplay/Game/Systems/SPlatforms.h"

#include "Gameplay/Systems/SEditorCamera.h"
#include "Gameplay/Systems/SRenderMeshes.h"
#include "Gameplay/Systems/SRenderCamera.h"
#include "Gameplay/Systems/SRenderLighting.h"
#include "Physics/2D/SPhysics2D.h"
#include "Physics/3D/SPhysics.h"

#include "Gameplay/Singletons/CActiveCamera.h"
#include "Gameplay/Singletons/CPhysicsWorld.h"
#include "Gameplay/Singletons/CVisualDebugger.h"


void ECSManager::BeginPlay()
{
	IterateSystems([](Ptr<System> system) {
		system->BeginPlay();
	});
}

void ECSManager::Tick(float deltaTime)
{
	auto world = GetWorld();

	IterateSystems([world, deltaTime](Ptr<System> system)
	{
		if (!world->IsEditor() || system->bTickOnEditor)
		{
			system->Tick(deltaTime);
		}
	});
}

EntityId ECSManager::CreateEntity(Name entityName, bool bTransient /*= false*/)
{
	const EntityId id = __CreateEntity(entityName);

	// Cache the created Guid
	const Guid guid = Get<CEntity>(id).id;
	guidEntityCache.insert_or_assign(guid, id);

	return id;
}

void ECSManager::DestroyEntity(EntityId entity)
{
	if (registry.valid(entity))
	{
		const Guid guid = Get<CEntity>(entity).id;
		__DestroyEntity(entity);

		// Remove the associated Guid
		guidEntityCache.erase(guid);
	}
}

EntityId ECSManager::__CreateEntity(Name entityName, bool bTransient /*= false*/)
{
	EntityId entity = registry.create();
	Assign<CEntity>(entity, entityName, bTransient);

	onEntityCreated.DoBroadcast(entity);
	return entity;
}

void ECSManager::__DestroyEntity(EntityId entity)
{
	onEntityDestroyed.DoBroadcast(entity);
	registry.destroy(entity);
}

bool ECSManager::Serialize(Archive& ar)
{
	bool bResult = Super::Serialize(ar);

	ar.BeginObject("entities");
	if (ar.IsLoading())
	{
		// Destroy all entities
		{
			View<CEntity>().each([this](EntityId id, CEntity entity)
			{
				onEntityDestroyed.DoBroadcast(id);
			});
			registry.reset();
		}

		u32 size;
		ar.SerializeArraySize(size);

		registry.reserve(size);

		TArray<EntityId> createdEntities;
		createdEntities.Reserve(size);

		// Reset Guid cache
		guidEntityCache.clear();
		guidEntityCache.reserve(size);

		// Create all entities
		for (u32 i = 0; i < size; ++i)
		{
			ar.BeginObject(i);
			if (ar.IsObjectValid())
			{
				EntityId id = __CreateEntity("None");

				// Serialize Id and cache it
				auto& entityComp = Assign<CEntity>(id);
				ar("id", entityComp.id);
				guidEntityCache.insert_or_assign(entityComp.id, id);

				createdEntities.Add(id);
			}
			ar.EndObject();
		}

		// Deserialize all entities
		for (u32 i = 0; i < size; ++i)
		{
			ar.BeginObject(i);
			if (ar.IsObjectValid())
			{
				SerializeEntity(ar, createdEntities[i]);
			}
			ar.EndObject();
		}
	}
	else
	{
		u32 size = (u32)registry.alive();
		ar.SerializeArraySize(size);

		u32 i = 0;
		View<CEntity>().each([this, &ar, &i](EntityId id, CEntity entity)
		{
			if (!entity.bTransient)
			{
				ar.BeginObject(i);
				{
					ar("id", entity.id);
					SerializeEntity(ar, id);
				}
				ar.EndObject();
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
	SerializeComponent<CCamera>(ar, entity);
	SerializeComponent<CEditorCamera>(ar, entity);
	SerializeComponent<CPointLight>(ar, entity);
	SerializeComponent<CDirectionalLight>(ar, entity);

	// Physics Components
	SerializeComponent<CBody2D>(ar, entity);
	SerializeComponent<CCollider2D>(ar, entity);
	SerializeComponent<CBoxCollider2D>(ar, entity);
	SerializeComponent<CCircleCollider2D>(ar, entity);
	SerializeComponent<CBody>(ar, entity);

	// Game
	SerializeComponent<CPlayer>(ar, entity);
	SerializeComponent<CPlatform>(ar, entity);
}

void ECSManager::RegistrySystems()
{
	// #TODO: Externalize system registry
	RegistrySystem<SPhysics2D>();
	RegistrySystem<SPhysics>();

	RegistrySystem<SEditorCamera>();
	RegistrySystem<SLighting>();
	RegistrySystem<SPlayer>();
	RegistrySystem<SPlatforms>();

	// Rendering
	RegistrySystem<SRenderCamera>();
	RegistrySystem<SRenderLighting>();
	RegistrySystem<SRenderMeshes>();
}

void ECSManager::RegistrySingletons()
{
	AssignSingleton<CActiveCamera>();
	AssignSingleton<CPhysicsWorld>();
	AssignSingleton<CVisualDebugger>();
}
