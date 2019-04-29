// Copyright 2015-2019 Piperift - All rights reserved

#include "PhysicsManager.h"


void PhysicsManager::Initialize()
{
	physics2D = Create<Physics2D>(Self());
	physics2D->Initialize();
}

void PhysicsManager::Tick(float deltaTime)
{
	physics2D->Tick(deltaTime);
}
