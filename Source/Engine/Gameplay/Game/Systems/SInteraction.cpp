// Copyright 2015-2019 Piperift - All rights reserved

#include "SInteraction.h"
#include "Core/Log.h"
#include "Core/Engine.h"
#include "../Components/CKey.h"
#include "../Components/CDoor.h"
#include "../Components/CElevator.h"
#include "../Components/CPlayer.h"
#include "Physics/3D/Components/CBody.h"
#include "Gameplay/Singletons/CPhysicsWorld.h"


void SInteraction::BeginPlay()
{
	Super::BeginPlay();

	// Assign closed locations
	auto view = ECS()->View<CDoor, CTransform>();
	for (EntityId entity : view)
	{
		CDoor& door = view.get<CDoor>(entity);
		CTransform& transform = view.get<CTransform>(entity);

		door.closedLocation = transform.GetWLocation();
	}
}

void SInteraction::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	auto ecs = ECS();
	CPhysicsWorld* physics = ecs->FindSingleton<CPhysicsWorld>();

	pickedKeys.Empty();

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

	UpdateDoors(deltaTime);
	UpdateElevators(deltaTime);
}

void SInteraction::UpdateDoors(float deltaTime)
{
	auto view = ECS()->View<CDoor, CTransform>();

	if (pickedKeys.Size() > 0)
	{
		// Unlock Doors
		for (EntityId entity : view)
		{
			CDoor& door = view.get<CDoor>(entity);
			if (door.GetState() == EDoorState::Closed &&
				pickedKeys.Contains(door.keyId))
			{
				door.state = u8(EDoorState::Opening);
			}
		}
	}

	// Update Doors position
	for (EntityId entity : view)
	{
		CDoor& door = view.get<CDoor>(entity);

		const EDoorState state = door.GetState();
		if (state == EDoorState::Opening || state == EDoorState::Closing)
		{
			CTransform& trans = view.get<CTransform>(entity);

			v3 target = door.closedLocation;
			if (state == EDoorState::Opening)
			{
				target += door.openedOffset;
			}

			const v3 ab = target - trans.GetWLocation();
			if (ab.LengthSqrt() < 0.1f * 0.1f)
			{
				// Finish opening or closing
				door.state = u8((state == EDoorState::Opening)? EDoorState::Opened : EDoorState::Closed);
			}
			else
			{
				// Velocity = Norm(B-A) * Speed
				const v3 speed = ab.Normalize() * door.speed * deltaTime;

				// Kinematic bodies cant use velocity directly
				trans.SetWLocation(trans.GetWLocation() + speed);
			}
		}
	}
}

void SInteraction::UpdateElevators(float deltaTime)
{
	// If any key was picked
	if (pickedKeys.Size() > 0)
	{
		// Activate Elevators
		auto enableView = ECS()->View<CElevator, CTransform, CBody>();
		for (EntityId entity : enableView)
		{
			CElevator& elev = enableView.get<CElevator>(entity);
			if (!elev.bEnabled && pickedKeys.Contains(elev.keyId))
			{
				elev.bEnabled = true;
			}
		}
	}

	// Update Elevators
	auto view = ECS()->View<CElevator, CTransform, CBody>();
	for (EntityId entity : view)
	{
		CElevator& elev = view.get<CElevator>(entity);
		if (elev.bEnabled)
		{
			CTransform& trans = view.get<CTransform>(entity);

			const v3 target = elev.bReturning ? elev.firstPosition : elev.secondPosition;

			const v3 ab = target - trans.GetWLocation();
			if (ab.LengthSqrt() < 0.1f * 0.1f)
			{
				// Point reached, switch target
				elev.bReturning = !elev.bReturning;
			}

			// Velocity = Norm(B-A) * Speed
			const v3 speed = ab.Normalize() * elev.speed * deltaTime;

			// Kinematic bodies cant use velocity directly
			trans.SetWLocation(trans.GetWLocation() + speed);
			//body.SetLinearVelocity(ab.Normalize() * elev.speed);
		}
	}
}
