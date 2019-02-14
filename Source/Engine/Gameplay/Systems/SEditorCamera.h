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

	EntityId camera;

	EventHandle onForward;
	EventHandle onSide;

public:

	SEditorCamera() : Super() {}

	virtual void BeginPlay() override;
	virtual void BeforeDestroy() override;

private:

	void OnMove(v3 delta);
};
