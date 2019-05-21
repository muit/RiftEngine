// Copyright 2015-2019 Piperift - All rights reserved

#include "SPlayer.h"
#include "Core/Log.h"

#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CCamera.h"

#include "Core/Engine.h"
#include "../Components/CPlayer.h"
#include "Physics/2D/Components/CBody2D.h"
#include "Physics/3D/Components/CBody.h"
#include "Tools/Profiler.h"


void SPlayer::BeginPlay()
{
	Super::BeginPlay();

	input = GEngine->GetInput();

	input->CreateAxisAction({ "MoveRight" }, {
		{ EKey::A, EKeyModifier::None, -1.f },
		{ EKey::D, EKeyModifier::None,  1.f }
	}, {})
	.Bind(this, &SPlayer::MoveRight);

	// Store home locations
	auto view = ECS()->View<CPlayer, CTransform>();
	for (EntityId entity : view)
	{
		CPlayer&    player    = view.get<CPlayer>(entity);
		const CTransform& transform = view.get<CTransform>(entity);

		player.homeLocation = transform.GetWLocation();
	}
}

void SPlayer::Tick(float deltaTime)
{
	ScopedGameZone("Player");

	auto ecs = ECS();

	const v2 finalMoveDelta = movementDelta;

	EntityId cameraTarget = NoEntity;
	CPlayer* playerPtr = nullptr;

	// Player movement 2D
	auto movement2DView = ecs->View<CPlayer, CBody2D>();
	for (EntityId entity : movement2DView)
	{
		CPlayer& player = movement2DView.get<CPlayer>(entity);
		CBody2D& body   = movement2DView.get<CBody2D>(entity);
		body.body.ApplyLinearImpulse({ finalMoveDelta * player.impulse });
		body.body.ApplyTorque(finalMoveDelta.x * player.impulse);
	}

	// Player movement 3D
	/*auto movement3DView = ecs->View<CPlayer, CBody>();
	for (EntityId entity : movement3DView)
	{
		CPlayer& player = movement3DView.get<CPlayer>(entity);
		CBody& body = movement3DView.get<CBody>(entity);
	}*/

	// Player Dead
	auto playerDeadView = ecs->View<CPlayer, CTransform>();
	for (EntityId entity : playerDeadView)
	{
		CPlayer&    player    = playerDeadView.get<CPlayer>(entity);
		CTransform& transform = playerDeadView.get<CTransform>(entity);

		if (transform.GetWLocation().z <= player.dieHeight)
		{
			Log::Message("Player died");
			transform.SetWLocation(player.homeLocation);
		}
	}

	// Find camera target
	auto camTargetView = ecs->View<CPlayer>();
	for (EntityId entity : camTargetView)
	{
		if (cameraTarget == NoEntity)
		{
			cameraTarget = entity;
			playerPtr = &camTargetView.get(entity);
		}
	}

	// Smooth camera follow
	if (cameraTarget != NoEntity && ecs->Has<CTransform>(cameraTarget))
	{
		auto cameraView = ECS()->View<CTransform, CCamera>();
		for (EntityId entity : cameraView)
		{
			CTransform& targetTransform = ecs->Get<CTransform>(cameraTarget);
			CTransform& transform = cameraView.get<CTransform>(entity);

			const v3 targetLocation = targetTransform.GetWLocation() + v3{ 0.f, -playerPtr->cameraDistance, 0.f };
			transform.SetWLocation(
				Math::Lerp(transform.GetWLocation(), targetLocation, deltaTime * playerPtr->cameraSpeed)
			);
		}
	}

	movementDelta = v2::Zero();
}

void SPlayer::MoveRight(float delta)
{
	movementDelta.x += delta;
}
