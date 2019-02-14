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

	onForward = input->CreateAxisAction({ "Forward" }, {
		{ EKey::W, EKeyModifier::None, -1.f },
		{ EKey::S, EKeyModifier::None,  1.f }
	}, {})
	.Bind([this](float value) {
		OnMove({ 0.f, 0.f, value });
	});

	onSide = input->CreateAxisAction({ "Side" }, {
		{ EKey::A, EKeyModifier::None,  1.f },
		{ EKey::D, EKeyModifier::None, -1.f }
	}, {})
	.Bind([this](float value) {
		OnMove({ value, 0.f, 0.f });
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

void SEditorCamera::BeforeDestroy()
{
	// #TODO: Find a better syntax
	GEngine->Input()->FindAxisAction({ "Forward" })->Unbind(onForward);
	GEngine->Input()->FindAxisAction({ "Side" })->Unbind(onSide);

	ECS()->DestroyEntity(camera);
	Super::BeforeDestroy();
}

void SEditorCamera::OnMove(v3 delta)
{
	// #TODO: Move to tick with a cached move vector
	ECS()->View<CTransform, CEditorCamera>()
		.each([delta](const EntityId e, CTransform& t, CEditorCamera& c)
	{
		// #TODO: Make delta time accessible from here
		t.transform.location += delta * 0.05f;
	});
}
