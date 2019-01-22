// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderMesh.h"
#include "Rendering/Commands/ResourceCommands.h"
#include "Core/World.h"
#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CMesh.h"


void SRenderMesh::BeginPlay()
{
	Super::BeginPlay();
	texture.Load();
}

void SRenderMesh::Tick(float /*deltaTime*/)
{
	ECS()->View<CTransform, CMesh>().each(
		[](const EntityId e, CTransform& t, CMesh& c) {

		}
	);

	if (texture)
	{
		GetWorld()->QueueRender<LoadTextureCommand>(0, texture->GetTextureData());
		GetWorld()->QueueRender<DrawTextureCommand>(0, v2_u32{ 10, 20 });
	}
}

void SRenderMesh::BeforeDestroy()
{
	Super::BeforeDestroy();
}
