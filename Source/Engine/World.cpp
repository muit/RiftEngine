// Copyright 2015-2019 Piperift - All rights reserved

#include "World.h"
#include "../Tools/Profiler.h"

void World::Initialize()
{
	Log::Message("World Start-Up");

	physics = Create<PhysicsManager>(Self());
	physics->Initialize();
	ecs = Create<ECSManager>(Self());

	scene = { "empty_scene.meta" };
	scene.LoadOrCreate();

	ecs->BeginPlay();
}

void World::Tick(float deltaTime)
{
	ScopedGameZone("World");

	ecs->Tick(deltaTime);
}
