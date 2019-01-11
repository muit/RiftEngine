// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderMesh.h"
#include "Core/Rendering/Commands/ResourceCommands.h"
#include "World/World.h"


void SRenderMesh::BeginPlay()
{
	Super::BeginPlay();
	texture.Load();
}

void SRenderMesh::Tick(float /*deltaTime*/)
{
	if (texture)
	{
		GetWorld()->QueueRender<LoadTextureCommand>(0, texture->GetTextureData());
		GetWorld()->QueueRender<DrawTextureCommand>(0, v2_u32{ 0, 0 });
	}
}

void SRenderMesh::BeforeDestroy()
{
	Super::BeforeDestroy();
}
