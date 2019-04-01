// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderMeshes.h"
#include "Rendering/Commands/ResourceCommands.h"
#include "Core/Engine.h"
#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CMesh.h"


void SRenderMeshes::BeginPlay()
{
	Super::BeginPlay();
	texture.Load();
}

void SRenderMeshes::Tick(float /*deltaTime*/)
{
	ECS()->View<CTransform, CMesh>().each(
	[](const EntityId e, CTransform& t, CMesh& c)
	{
		if (c.model.IsValid())
		{
			//QueueRenderCommand<DrawMeshCommand>(c.model->GetId(), t.transform, c.color);
		}
	});

	if (texture)
	{
		//QueueRenderCommand<Draw2DTextureCommand>(0, v2_u32{ 10, 20 });
	}
}

void SRenderMeshes::BeforeDestroy()
{
	Super::BeforeDestroy();
}
