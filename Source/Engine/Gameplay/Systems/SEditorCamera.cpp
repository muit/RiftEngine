// Copyright 2015-2019 Piperift - All rights reserved

#include "SEditorCamera.h"
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
	ECS()->View<CTransform, CEditorCamera>().each(
		[deltaTime](const EntityId e, CTransform& t, CEditorCamera& c)
		{
			t.transform.location.x() += 0.5f * deltaTime;
		}
	);
}

void SEditorCamera::BeforeDestroy()
{
	ECS()->DestroyEntity(camera);
	Super::BeforeDestroy();
}
