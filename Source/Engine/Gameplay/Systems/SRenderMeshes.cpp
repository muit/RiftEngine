// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderMeshes.h"
#include "Core/Engine.h"
#include "Tools/Profiler.h"
#include "Assets/Texture.h"

#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CMesh.h"
#include "Gameplay/Singletons/CGraphics.h"

#include "Rendering/Commands/MeshCommands.h"
#include "Rendering/Commands/SkyboxCommand.h"


const TAssetPtr<Material> SRenderMeshes::skyboxMaterial{ "Shaders/skybox.shader.meta" };
const TAssetPtr<Mesh> SRenderMeshes::skyboxCube{ "Meshes/cube.obj.meta" };

SRenderMeshes::SRenderMeshes()
	: Super()
{
	bTickOnEditor = true;

	Texture::default.Load();
	skyboxMaterial.Load();
	skyboxCube.Load();
}

void SRenderMeshes::Tick(float deltaTime)
{
	ScopedStackGameZone();
	Super::Tick(deltaTime);

	DrawSkybox();


	auto view = ECS()->View<CTransform, CMesh>();

	TArray<MeshDrawInstance> meshInstances;
	meshInstances.Reserve((i32)view.size());

	view.each([&meshInstances](const EntityId e, CTransform& t, CMesh& c)
	{
		if (!c.model.IsNull())
		{
			TAssetPtr<Material> material;
			if (c.overrideMaterial.IsNull())
			{
				material = c.model->material;
			}
			else
			{
				c.overrideMaterial.Load();
				material = c.overrideMaterial;
			}

			meshInstances.Add({
				c.model.GetInfo(),
				material.GetInfo(),
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

void SRenderMeshes::DrawSkybox()
{
	CGraphics* graphics = ECS()->FindSingleton<CGraphics>();

	if(!graphics->cubeMap.IsNull())
	{
		// Ensure it's loaded
		if (graphics->cubeMap.Load())
		{
			QueueRenderCommand<DrawSkyboxCommand>(
				skyboxCube.GetInfo(),
				skyboxMaterial.GetInfo(),
				graphics->cubeMap.GetInfo()
			);
		}
	}
}
