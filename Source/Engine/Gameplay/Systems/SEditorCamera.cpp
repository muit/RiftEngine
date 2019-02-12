// Copyright 2015-2019 Piperift - All rights reserved

#include "SEditorCamera.h"
#include "Core/Log.h"

#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CEditorCamera.h"

#include "Core/Engine.h"


void SEditorCamera::BeginPlay()
{
	Super::BeginPlay();

	input = GEngine->Input();

	// #TODO: "Input Actions" implementation for better syntax and avoiding multiple bindings
	onW = input->OnKeyPressed(EKey::W).Bind([this](EKey, EKeyModifier) {
		OnMoveForward(-1.f);
	});
	onS = input->OnKeyPressed(EKey::S).Bind([this](EKey, EKeyModifier) {
		OnMoveForward(1.f);
	});

	onA = input->OnKeyPressed(EKey::A).Bind([this](EKey, EKeyModifier) {
		OnMoveSide(1.f);
	});
	onD = input->OnKeyPressed(EKey::D).Bind([this](EKey, EKeyModifier) {
		OnMoveSide(-1.f);
	});


	// If there's no camera, create one
	auto view = ECS()->View<CEditorCamera>();
	if (view.empty())
	{
		camera = ECS()->CreateEntity({ "EditorCamera" }, true);

		auto& t = ECS()->Assign<CTransform>(camera).transform;
		t.location = { 10, 3, 10 };
		t.SetRotationDegrees({ -22.5f, 22.5f, 180 });

		ECS()->Assign<CEditorCamera>(camera);
	}
}

void SEditorCamera::Tick(float deltaTime)
{
}

void SEditorCamera::BeforeDestroy()
{
	GEngine->Input()->OnKeyPressed(EKey::W).Unbind(onW);
	GEngine->Input()->OnKeyPressed(EKey::S).Unbind(onS);
	GEngine->Input()->OnKeyPressed(EKey::A).Unbind(onA);
	GEngine->Input()->OnKeyPressed(EKey::D).Unbind(onD);

	ECS()->DestroyEntity(camera);
	Super::BeforeDestroy();
}

void SEditorCamera::OnMoveForward(float delta)
{
	ECS()->View<CTransform, CEditorCamera>()
		.each([delta](const EntityId e, CTransform& t, CEditorCamera& c)
	{
		// #TODO: Make delta time accessible from here
		// Rotate camera each frame
		t.transform.location += v3{ 0, 0, delta * 0.05f };
	});
}

void SEditorCamera::OnMoveSide(float delta)
{
	// #TODO: Move to tick with a cached move vector
	ECS()->View<CTransform, CEditorCamera>()
		.each([delta](const EntityId e, CTransform& t, CEditorCamera& c)
	{
		// #TODO: Make delta time accessible from here
		t.transform.location += v3{ delta * 0.05f, 0, 0 };
	});
}
