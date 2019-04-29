// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderMeshes.h"
#include "Rendering/Commands/MeshCommands.h"
#include "Core/Engine.h"
#include "Tools/Profiler.h"

#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CMesh.h"


void SRenderMeshes::BeginPlay()
{
	Super::BeginPlay();
	texture.Load();
}

void SRenderMeshes::Tick(float deltaTime)
{
	ScopedStackGameZone();
	Super::Tick(deltaTime);

	auto view = ECS()->View<CTransform, CMesh>();

	TArray<MeshDrawInstance> meshInstances;
	meshInstances.Reserve((i32)view.size());

	view.each([&meshInstances](const EntityId e, CTransform& t, CMesh& c)
	{
		if (c.model.IsValid())
		{
			meshInstances.Add({
				c.model.GetInfo(),
				c.model->material.GetInfo(),
				t.worldTransform
			});
		}
	});

	QueueRenderCommand<DrawMeshesCommand>(MoveTemp(meshInstances));

	if (texture)
	{
		//QueueRenderCommand<Draw2DTextureCommand>(0, v2_u32{ 10, 20 });
	}
}

void SRenderMeshes::BeforeDestroy()
{
	Super::BeforeDestroy();
}
