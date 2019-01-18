// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "ECS/ECSManager.h"


class SEditorCamera : public System {
	CLASS(SEditorCamera, System)

	EntityId camera;


public:

	SEditorCamera() : Super() {}

	virtual void BeginPlay() override;
	virtual void Tick(float /*deltaTime*/) override;
	virtual void BeforeDestroy() override;
};
