// Copyright 2015-2019 Piperift - All rights reserved

#include "SDoors.h"
#include "Core/Log.h"
#include "Core/Engine.h"
#include "../Components/CKey.h"
#include "../Components/CDoor.h"
#include "Physics/3D/Components/CBody.h"


void SDoors::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	TArray<Name> pickedKeys;

	// Items that got picked
	auto keysView = ECS()->View<CKey, CBody>();
	for (EntityId entity : keysView)
	{
		CKey& key = keysView.get<CKey>(entity);

		if(!key.bPickedUp /* && HasOverlap */)
		{
			pickedKeys.Add(key.keyId);
			key.bPickedUp = true;
		}
	}

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
