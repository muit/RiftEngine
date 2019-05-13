// Copyright 2015-2019 Piperift - All rights reserved

#include "SPlatforms.h"
#include "Core/Log.h"
#include "Core/Engine.h"
#include "../Components/CPlatform.h"
#include "Physics/2D/Components/CBody2D.h"


void SPlatforms::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	auto view = ECS()->View<CPlatform, CBody2D, CTransform>();
	for (EntityId entity : view)
	{
		CPlatform& platform = view.get<CPlatform>(entity);
		CBody2D& body = view.get<CBody2D>(entity);
		const CTransform& trans = view.get<CTransform>(entity);

		const v3 target = platform.bReturning ? platform.firstPosition : platform.secondPosition;

		const v3 ab = target - trans.GetWLocation();
		if (ab.LengthSqrt() < 0.1f * 0.1f)
		{
			platform.bReturning = !platform.bReturning;
		}
		// Norm(B-A) * Speed
		const v2 delta = ab.xz().Normalize() * platform.speed;

		body.body.SetLinearVelocity(delta);
	}
}
