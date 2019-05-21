// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "ECS/ECSManager.h"
#include "Core/Input/Keys.h"
#include "Core/Input/InputManager.h"


class SPlayer : public System {
	CLASS(SPlayer, System)

	Ptr<InputManager> input;

	v2 movementDelta{ v3::Zero() };


public:

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

private:

	void MoveRight(float delta);
};
