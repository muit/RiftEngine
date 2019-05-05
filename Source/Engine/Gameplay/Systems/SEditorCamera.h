// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "ECS/ECSManager.h"
#include "Core/Input/Keys.h"
#include "Core/Input/InputManager.h"


class SEditorCamera : public System {
	CLASS(SEditorCamera, System)

	Ptr<InputManager> input;

	// Cached camera entity
	EntityId camera = NoEntity;

	float forwardSpeed = 10.f;
	float sideSpeed = 5.f;
	float rotateSpeed = 10.f;

	bool bRotatingMode = false;
	v3 movementDelta{ v3::Zero() };
	v3 rotationDelta{ v3::Zero() };

public:

	SEditorCamera() : Super() { bTickOnEditor = true; }

	virtual void Construct() override;
	virtual void Tick(float deltaTime) override;
	virtual void BeforeDestroy() override;

private:

	void ViewportMoveMode(EKeyPressState state);
	void MoveForward(float delta);
	void MoveRight(float delta);
	void TurnUp(float delta);
	void TurnRight(float delta);
};
