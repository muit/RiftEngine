// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"


class SRenderMesh : public System {
	CLASS(SRenderMesh, System)

	virtual void Tick(float /*deltaTime*/) override;
};
