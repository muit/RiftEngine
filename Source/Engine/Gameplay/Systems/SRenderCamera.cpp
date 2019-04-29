// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderCamera.h"
#include "Core/Engine.h"
#include "Tools/Profiler.h"

#include "Gameplay/Components/CEditorCamera.h"


void SRenderCamera::Tick(float deltaTime)
{
	ScopedStackGameZone();
	Super::Tick(deltaTime);

	CameraData cameraData{};

	if (GetWorld()->IsEditor())
	{
		// Use first editor camera
		auto view = ECS()->View<CTransform, CEditorCamera>();
		if (!view.empty())
		{
			EntityId cameraEntity = *view.begin();

			if (ECS()->IsValid(cameraEntity)) {
				cameraData = GetCameraData(
					view.get<CTransform>(cameraEntity).GetWorldTransform(),
					&view.get<CEditorCamera>(cameraEntity)
				);
			}
		}
	}
	else
	{
		// Use first entity with Transform and Camera
		auto view = ECS()->View<CTransform, CCamera>();
		if (!view.empty())
		{
			EntityId cameraEntity = *view.begin();

			if (ECS()->IsValid(cameraEntity)) {
				cameraData = GetCameraData(
					view.get<CTransform>(cameraEntity).GetWorldTransform(),
					&view.get<CCamera>(cameraEntity)
				);
			}
		}
	}

	// Render camera
	QueueRenderCommand<CameraCommand>(cameraData);
}
