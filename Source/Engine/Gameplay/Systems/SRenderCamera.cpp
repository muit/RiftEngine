// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderCamera.h"
#include "Core/World.h"
#include "Gameplay/Components/CEditorCamera.h"


void SRenderCamera::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	CameraData cameraData;

	if (GetWorld()->IsEditor())
	{
		// Use first entity with Transform and Camera
		auto view = ECS()->View<CTransform, CEditorCamera>();
		if (!view.empty())
		{
			EntityId cameraEntity = *view.begin();

			if (ECS()->IsValid(cameraEntity)) {
				cameraData = GetCameraData(
					view.get<CTransform>(cameraEntity).transform,
					&view.get<CEditorCamera>(cameraEntity)
				);
			}
		}
	}
	else
	{
		// Use first editor camera
		auto view = ECS()->View<CTransform, CCamera>();
		if (!view.empty())
		{
			EntityId cameraEntity = *view.begin();

			if (ECS()->IsValid(cameraEntity)) {
				cameraData = GetCameraData(
					view.get<CTransform>(cameraEntity).transform,
					&view.get<CCamera>(cameraEntity)
				);
			}
		}
	}

	// Render camera
	GetWorld()->QueueRender<CameraCommand>(cameraData);
}
