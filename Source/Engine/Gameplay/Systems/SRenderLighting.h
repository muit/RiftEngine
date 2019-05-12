// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"


class SRenderLighting : public System {
	CLASS(SRenderLighting, System)

public:

	SRenderLighting() : Super() { bTickOnEditor = true; }

	virtual void Tick(float deltaTime) override;
};
