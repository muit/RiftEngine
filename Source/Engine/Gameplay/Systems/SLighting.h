// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "Rendering/Data/CameraData.h"
#include "Gameplay/Components/CCamera.h"
#include "Gameplay/Components/CTransform.h"


class SLighting : public System {
	CLASS(SLighting, System)

	virtual void Tick(float deltaTime) override;
};
