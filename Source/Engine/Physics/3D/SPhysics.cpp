// Copyright 2015-2019 Piperift - All rights reserved

#include "SPhysics.h"
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/PxFoundation.h>

#include "World.h"
#include "Core/MultiThreading.h"
#include "Tools/Profiler.h"

#include "PhysicsTypes.h"
#include "Gameplay/Singletons/CPhysicsWorld.h"


void SPhysics::BeginPlay()
{
	Super::BeginPlay();


	/*foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *allocator, getSampleErrorCallback());
	if (!foundation)
	{
		Log::Error("PxCreateFoundation failed!");
	}*/


	physicsWorld = ECS()->FindSingleton<CPhysicsWorld>();
}

void SPhysics::Tick(float deltaTime)
{
	ScopedStackGameZone();
	Super::Tick(deltaTime);

	Step(deltaTime);
}

void SPhysics::EndPlay()
{
	Super::EndPlay();
}

void SPhysics::Step(float deltaTime)
{
	ScopedGameZone("Step");
}
