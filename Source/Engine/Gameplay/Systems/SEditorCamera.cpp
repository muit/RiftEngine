// Copyright 2015-2019 Piperift - All rights reserved

#include "SEditorCamera.h"
#include "Core/Log.h"

#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CEditorCamera.h"

#include "Core/Engine.h"


void SEditorCamera::Construct()
{
	Super::Construct();

	input = GEngine->GetInput();

	input->CreateTriggerAction({ "ViewportMoveMode" }, {
		{ EKey::MouseRight, EKeyModifier::None },
	})
	.Bind(this, &SEditorCamera::ViewportMoveMode);

	input->CreateAxisAction({ "MoveForward" }, {
		{ EKey::W, EKeyModifier::None,  1.f },
		{ EKey::S, EKeyModifier::None, -1.f }
	}, {})
	.Bind(this, &SEditorCamera::MoveForward);

	input->CreateAxisAction({ "MoveRight" }, {
		{ EKey::A, EKeyModifier::None, -1.f },
		{ EKey::D, EKeyModifier::None,  1.f }
	}, {})
	.Bind(this, &SEditorCamera::MoveRight);

	input->CreateAxisAction({ "TurnUp" }, {}, {
		{ EAxis::MouseY, EKeyModifier::None, 1.f }
	})
	.Bind(this, &SEditorCamera::TurnUp);

	input->CreateAxisAction({ "TurnRight" }, {}, {
		{ EAxis::MouseX, EKeyModifier::None, -1.f }
	})
	.Bind(this, &SEditorCamera::TurnRight);

	// If there's no camera, create one
	auto view = ECS()->View<CEditorCamera>();
	if (view.empty())
	{
		camera = ECS()->CreateEntity({ "EditorCamera" });

		auto& t = ECS()->Assign<CTransform>(camera).transform;
		t.location = { 0, -10, 0 };
		t.SetRotation({ 0.f, 0.f, 0.f });

		ECS()->Assign<CEditorCamera>(camera);
	}
}

void SEditorCamera::Tick(float deltaTime)
{
	// Only tick on editor
	if (!GetWorld()->IsEditor())
	{
		bRotatingMode = false;
		return;
	}

	const v3 finalRotateDelta = rotationDelta * deltaTime;
	const v3 finalMoveDelta = movementDelta * deltaTime;

	ECS()->View<CTransform, CEditorCamera>()
	.each([finalRotateDelta, finalMoveDelta](const auto e, CTransform& t, CEditorCamera& c)
	{
		// Rotate Camera
		Rotator rotation = t.transform.GetRotation();
		rotation += finalRotateDelta * c.rotateSpeed;

		// Limit vertical rotation
		rotation.x = Math::ClampAngle(rotation.x, 270.01f, 89.99f);
		t.transform.SetRotation(rotation);

		// Rotate movement towards angle
		t.transform.location += t.transform.TransformVector(
			finalMoveDelta * v3{ c.sideSpeed, c.forwardSpeed, 0.f }
		);
	});

	rotationDelta = v3::Zero();
	movementDelta = v3::Zero();
}

void SEditorCamera::BeforeDestroy()
{
	ECS()->DestroyEntity(camera);
	Super::BeforeDestroy();
}

void SEditorCamera::ViewportMoveMode(EKeyPressState state)
{
	switch (state) {
	case EKeyPressState::Press:
		bRotatingMode = true;
		SDL_SetRelativeMouseMode(SDL_TRUE);
		break;
	case EKeyPressState::Release:
		bRotatingMode = false;
		SDL_SetRelativeMouseMode(SDL_FALSE);
		break;
	}
}

void SEditorCamera::MoveForward(float delta)
{
	if (GetWorld()->IsEditor())
	{
		movementDelta.y += delta;
	}
}

void SEditorCamera::MoveRight(float delta)
{
	if (GetWorld()->IsEditor())
	{
		movementDelta.x += delta;
	}
}

void SEditorCamera::TurnUp(float delta)
{
	if (bRotatingMode)
	{
		rotationDelta.x += delta;
	}
}

void SEditorCamera::TurnRight(float delta)
{
	if (bRotatingMode)
	{
		rotationDelta.z += delta;
	}
}