// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderCamera.h"
#include "Core/World.h"
#include "Gameplay/Components/CCamera.h"
#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CEditorCamera.h"


void SRenderCamera::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	Transform cameraTransform {};

	if (GetWorld()->IsEditor())
	{
		// Use first editor camera
		auto view = ECS()->View<CTransform, CCamera>();
		if (!view.empty())
		{
			EntityId cameraEntity = *view.begin();

			if (ECS()->IsValid(cameraEntity)) {
				cameraTransform = view.get<CTransform>(cameraEntity).transform;
			}
		}
	}
	else
	{
		// Use first entity with Transform and Camera
		auto view = ECS()->View<CTransform, CEditorCamera>();
		if (!view.empty())
		{
			EntityId cameraEntity = *view.begin();

			if (ECS()->IsValid(cameraEntity)) {
				cameraTransform = view.get<CTransform>(cameraEntity).transform;
			}
		}
	}

	// Render camera
	GetWorld()->QueueRender<CameraCommand>(cameraTransform);
}
