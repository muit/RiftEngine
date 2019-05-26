// Copyright 2015-2019 Piperift - All rights reserved

#include "SDoors.h"
#include "Core/Log.h"
#include "Core/Engine.h"
#include "../Components/CKey.h"
#include "../Components/CDoor.h"
#include "../Components/CPlayer.h"
#include "Physics/3D/Components/CBody.h"
#include "Gameplay/Singletons/CPhysicsWorld.h"


void SDoors::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	auto ecs = ECS();
	CPhysicsWorld* physics = ecs->FindSingleton<CPhysicsWorld>();

	TArray<Name> pickedKeys;

	if (physics)
	{
		for (const auto& ev : physics->triggerEvents)
		{
			// Player entered inside a Key trigger
			if (ecs->Has<CPlayer>(ev.otherEntity) && ecs->Has<CKey>(ev.triggerEntity))
			{
				CKey& key = ecs->Get<CKey>(ev.triggerEntity);
				if (!key.bPickedUp)
				{
					pickedKeys.Add(key.keyId);
					key.bPickedUp = true;
				}
			}
		}
	}

	if (pickedKeys.Size() <= 0)
		return;

	// Update doors
	auto doorsView = ECS()->View<CDoor, CTransform>();
	for (EntityId entity : doorsView)
	{
		CDoor& door = doorsView.get<CDoor>(entity);

		// This door's key was picked, open it
		if(!door.bOpened && pickedKeys.Contains(door.keyId))
		{
			door.bOpened = true;
		}

		if(door.bOpened)
		{
			CTransform& transform = doorsView.get<CTransform>(entity);
			// Move tick
		}
	}
}
