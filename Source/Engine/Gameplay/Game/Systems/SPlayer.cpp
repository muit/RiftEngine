// Copyright 2015-2019 Piperift - All rights reserved

#include "SPlayer.h"
#include "Core/Log.h"

#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CCamera.h"

#include "Core/Engine.h"
#include "../Components/CPlayer.h"
#include "Physics/Components/CBody2D.h"
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
}

void SPlayer::Tick(float deltaTime)
{
	ScopedGameZone("Player");

	auto ecs = ECS();

	const v2 finalMoveDelta = movementDelta * deltaTime;

	EntityId cameraTarget = NoEntity;
	CPlayer* playerPtr;

	// Player movement
	auto playerView = ecs->View<CPlayer, CBody2D>();
	for (EntityId entity : playerView)
	{
		CPlayer& player = playerView.get<CPlayer>(entity);
		CBody2D& body = playerView.get<CBody2D>(entity);
		body.body.GetRaw()->ApplyLinearImpulseToCenter({ finalMoveDelta * player.impulse }, true);

		if (cameraTarget == NoEntity)
		{
			cameraTarget = entity;
			playerPtr = &player;
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

			const v3 targetLocation = transform.GetWLocation() + v3{ 0.f, -playerPtr->cameraDistance, 0.f };
			transform.SetWLocation(
				Math::Lerp(targetLocation, targetTransform.GetWLocation(), deltaTime * playerPtr->cameraSpeed)
			);
		}
	}

	movementDelta = v2::Zero();
}

void SPlayer::MoveRight(float delta)
{
	movementDelta.x += delta;
}
