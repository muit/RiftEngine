// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderVisualDebugger.h"
#include "ECS/ECSManager.h"
#include "Tools/Profiler.h"
//#include "Rendering/Commands/VisualDebuggerCommand.h"


void SRenderVisualDebugger::BeginPlay()
{
	Super::BeginPlay();

	visualDebugger = ECS()->FindSingleton<CVisualDebugger>();
}

void SRenderVisualDebugger::Tick(float deltaTime)
{
	ScopedStackGameZone();
	Super::Tick(deltaTime);

	if (!visualDebugger)
	{
		return;
	}

	// Swap record and render caches
	renderQueue->Reset();
	eastl::swap(visualDebugger->GetQueue(), renderQueue);


	//QueueRenderCommand<DrawMeshesCommand>(MoveTemp(meshInstances));
}
