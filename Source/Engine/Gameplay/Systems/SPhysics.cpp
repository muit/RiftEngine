// Copyright 2015-2019 Piperift - All rights reserved

#include "SPhysics.h"
#include "Tools/Profiler.h"
#include "Core/World.h"


void SPhysics::Tick(float deltaTime)
{
	ScopedStackGameZone();
	Super::Tick(deltaTime);

	// Update physics data
	GetWorld()->GetPhysics()->Tick(deltaTime);
	// Apply physics data
}
