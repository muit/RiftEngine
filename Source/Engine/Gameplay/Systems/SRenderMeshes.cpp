// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderMeshes.h"
#include "Rendering/Commands/MeshCommands.h"
#include "Core/Engine.h"
#include "Tools/Profiler.h"
#include "Assets/Texture.h"

#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CMesh.h"


void SRenderMeshes::BeginPlay()
{
	Super::BeginPlay();

	Texture::default.Load();
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
		if (!c.model.IsNull())
		{
			meshInstances.Add({
				c.model.GetInfo(),
				c.model->material.GetInfo(),
				t.transform
			});
		}
	});

	QueueRenderCommand<DrawMeshesCommand>(MoveTemp(meshInstances));
}

void SRenderMeshes::BeforeDestroy()
{
	Super::BeforeDestroy();
}
