// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderMeshes.h"
#include "Rendering/Commands/ResourceCommands.h"
#include "Core/World.h"
#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CMesh.h"


void SRenderMeshes::BeginPlay()
{
	Super::BeginPlay();
	texture.Load();
}

void SRenderMeshes::Tick(float /*deltaTime*/)
{
	Ptr<World> world{ GetWorld() };

	ECS()->View<CTransform, CMesh>().each(
	[world](const EntityId e, CTransform& t, CMesh& c)
	{
		if (!c.model.IsNull())
		{
			if (!c.model.IsValid())
			{
				c.model.Load();
				world->QueueRender<LoadMeshCommand>(0, c.model->GetMeshData());
			}

			world->QueueRender<DrawMeshCommand>(0, t.transform);

			// #TODO: Unload models if needed
		}
	});

	if (texture)
	{
		world->QueueRender<LoadTextureCommand>(0, texture->GetTextureData());
		world->QueueRender<DrawTextureCommand>(0, v2_u32{ 10, 20 });
	}
}

void SRenderMeshes::BeforeDestroy()
{
	Super::BeforeDestroy();
}
