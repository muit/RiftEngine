// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "Gameplay/Components/CCamera.h"
#include "Gameplay/Components/CTransform.h"


class SRenderCamera : public System {
	CLASS(SRenderCamera, System)

	virtual void Tick(float deltaTime) override{
		Super::Tick(deltaTime);

		ECS()->View<CTransform, CCamera>().each(
			[](const EntityId e, CTransform& t, CCamera& c) {

			}
		);
	}
};
