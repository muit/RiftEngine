// Copyright 2015-2019 Piperift - All rights reserved

#include "SRotatingCubes.h"
#include "Core/Log.h"
#include "Core/Engine.h"
#include "Physics/3D/Components/CBody.h"
#include "Gameplay/Singletons/CPhysicsWorld.h"
#include "../Components/CRotatingCube.h"
#include "../Singletons/CSquare.h"


void SRotatingCubes::BeginPlay()
{
	auto ecs = ECS();
	squareComp = ecs->FindSingleton<CSquare>();

	if (squareComp)
	{
		u8 currentSegment = 0;
		u8 currentCount = 0;
		const u8 cubesPerSegment = squareComp->cubeCount / 4;

		for (u8 i = 0; i < squareComp->cubeCount; ++i)
		{
			const u8 lastSegment = (currentSegment + 1) % 4;

			// Create a cube and move it to initial position
			EntityId e = ecs->CreateEntity("Cube", true);
			ecs->Assign<CMesh>(e, TAssetPtr<Mesh>{"Meshes/box.fbx.meta"});
			CRotatingCube& cube = ecs->Assign<CRotatingCube>(e);
			CTransform& trans = ecs->Assign<CTransform>(e);

			const v3 corner = GetSegmentPosition(currentSegment);
			const v3 nextCorner = GetSegmentPosition(lastSegment);
			const v3 ab = nextCorner - corner;
			const float distance = (1.f / cubesPerSegment) * currentCount;

			trans.SetWLocation(corner + ab * distance);
			cube.segment = lastSegment;

			++currentCount;
			if (currentCount >= cubesPerSegment)
			{
				++currentSegment;
				currentCount = 0;
			}
		}
	}
}

void SRotatingCubes::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (!squareComp)
		return;

	auto ecs = ECS();
	auto view = ecs->View<CRotatingCube, CTransform>();

	for (EntityId entity : view)
	{
		CRotatingCube& cube = view.get<CRotatingCube>(entity);
		CTransform& transform = view.get<CTransform>(entity);

		const v3 origin = transform.GetWLocation();
		const v3 target = GetSegmentPosition(cube.segment);
		const v3 direction = target - origin;
		const v3 moveDelta = direction.Normalize() * cube.speed * deltaTime;

		transform.SetWLocation(origin + moveDelta);

		// We are close to the final point
		if (direction.LengthSqrt() < 0.025f)
		{
			if (cube.segment >= 3)
				cube.segment = 0;
			else
				cube.segment += 1;
		}
	}
}

v3 SRotatingCubes::GetSegmentPosition(u8 segment) const
{
	assert(squareComp);

	switch (segment)
	{
	case 0:
		return squareComp->cornerA;
	case 1:
		return squareComp->cornerB;
	case 2:
		return squareComp->cornerC;
	case 3:
		return squareComp->cornerD;
	default:
		return v3::Zero();
	}
}
