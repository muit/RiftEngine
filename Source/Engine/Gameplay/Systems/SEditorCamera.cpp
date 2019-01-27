// Copyright 2015-2019 Piperift - All rights reserved

#include "SEditorCamera.h"
#include "Core/Log.h"

#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CEditorCamera.h"


void SEditorCamera::BeginPlay()
{
	Super::BeginPlay();


	// If there's no camera, create one
	auto view = ECS()->View<CEditorCamera>();
	if (view.empty())
	{
		camera = ECS()->CreateEntity({ "EditorCamera" });

		auto& t = ECS()->Assign<CTransform>(camera).transform;
		t.location = { 10, 3, 10 };
		t.SetRotationDegrees({ -22.5f, 22.5f, 180 });

		ECS()->Assign<CEditorCamera>(camera);
	}
}

void SEditorCamera::Tick(float deltaTime)
{
	ECS()->View<CTransform, CEditorCamera>()
	.each([deltaTime](const EntityId e, CTransform& t, CEditorCamera& c)
	{
		// Rotate camera each frame
		t.transform.location += v3{0, 0.01f, 0};
	});
}

void SEditorCamera::BeforeDestroy()
{
	ECS()->DestroyEntity(camera);
	Super::BeforeDestroy();
}
