// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "ECS/ECSManager.h"
#include "Core/Input/Keys.h"
#include "Core/Input/InputManager.h"


class SPlatforms : public System {
	CLASS(SPlatforms, System)

public:

	virtual void Tick(float deltaTime) override;
};
