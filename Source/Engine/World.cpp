// Copyright 2015-2019 Piperift - All rights reserved

#include "World.h"
#include "../Tools/Profiler.h"

void World::Tick(float deltaTime)
{
	ScopedGameZone("World");

	ecs->Tick(deltaTime);
}
