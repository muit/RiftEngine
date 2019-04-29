// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "Gameplay/Components/CTransform.h"


class SPhysics : public System {
	CLASS(SPhysics, System)

	virtual void Tick(float deltaTime) override;
};
