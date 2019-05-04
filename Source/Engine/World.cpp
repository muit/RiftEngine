// Copyright 2015-2019 Piperift - All rights reserved

#include "World.h"
#include "../Tools/Profiler.h"
#include "Core/Engine.h"


void World::Initialize()
{
	Log::Message("World Start-Up");

	ecs = Create<ECSManager>(Self());

	scene = { "empty_scene.meta" };
	scene.LoadOrCreate();

#if WITH_EDITOR
	worldType = EWorldType::Editor;
	ecs->Initialize();
#else
	// BeginPlay straight away
	worldType = EWorldType::Standalone;
	ecs->Initialize();
	BeginPlay();
#endif
}

void World::BeginPlay()
{
	if (IsPlaying())
		return;

	ScopedGameZone("BeginPlay");
	worldType = IsEditor() ? EWorldType::PIE : EWorldType::Standalone;

	ecs->BeginPlay();
}

void World::Tick(float deltaTime)
{
	ScopedGameZone("World");

	ecs->Tick(deltaTime);
}

void World::EndPlay()
{
	ScopedGameZone("End Play");

	ecs->EndPlay();

	if (IsPIE())
	{
		worldType = EWorldType::Editor;
	}
	else
	{
		worldType = EWorldType::EndingPlay;
		GEngine->Shutdown();
	}
}

void World::Shutdown()
{
	if (IsPIE() || IsStandalone())
	{
		EndPlay();
	}
}
