// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderCamera.h"
#include "World/World.h"
#include "Gameplay/Components/CCamera.h"
#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CEditorCamera.h"
#include "Gameplay/Singletons/CGlobalCamera.h"


void SRenderCamera::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	Transform cameraTransform {};

	if (GetWorld()->IsEditor())
	{
		// Use first entity with Transform and Camera
		auto view = ECS()->View<CTransform, CEditorCamera>();
		EntityId cameraEntity = *view.begin();

		if (ECS()->IsValid(cameraEntity)) {
			cameraTransform = view.get<CTransform>(cameraEntity).transform;
		}
	}
	else
	{
		// Use first editor camera
		auto view = ECS()->View<CTransform, CCamera>();
		EntityId cameraEntity = *view.begin();

		if (ECS()->IsValid(cameraEntity)) {
			cameraTransform = view.get<CTransform>(cameraEntity).transform;
		}
	}

	// Render camera
	ECS()->FindSingleton<CGlobalCamera>()->transform = cameraTransform;
}
