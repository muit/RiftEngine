// Copyright 2015-2019 Piperift - All rights reserved

#include "SEditorCamera.h"
#include "Core/Log.h"

#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CEditorCamera.h"


void SEditorCamera::BeginPlay()
{
	Super::BeginPlay();

	camera = ECS()->CreateEntity({ "EditorCamera" });
	ECS()->Assign<CTransform>(camera);
	ECS()->Assign<CEditorCamera>(camera);
}

void SEditorCamera::Tick(float deltaTime)
{
	ECS()->View<CTransform, CEditorCamera>()
	.each([deltaTime](const EntityId e, CTransform& t, CEditorCamera& c)
	{
		// Rotate camera each frame
		v3 rotation = t.transform.GetRotationDegrees();
		rotation.z() += 5.f * deltaTime;
		t.transform.SetRotationDegrees(rotation);

		Log::Message("Camera Rotation: (%f, %f, %f)", rotation.x(), rotation.y(), rotation.z());
	});
}

void SEditorCamera::BeforeDestroy()
{
	ECS()->DestroyEntity(camera);
	Super::BeforeDestroy();
}
