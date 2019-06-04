// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderSpiral.h"
#include "Core/Engine.h"
#include "../SpiralCommand.h"


void SRenderSpiral::BeginPlay()
{
	QueueRenderCommand<LoadSpiralCommand>(1000);
	material.Load();
}

void SRenderSpiral::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	QueueRenderCommand<DrawSpiralCommand>(material.GetInfo());
}
