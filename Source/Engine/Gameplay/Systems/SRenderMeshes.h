// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "Core/Assets/AssetPtr.h"


class SRenderMeshes : public System {
	CLASS(SRenderMeshes, System)


public:

	SRenderMeshes() : Super() { bTickOnEditor = true; }

	virtual void BeginPlay() override;
	virtual void Tick(float /*deltaTime*/) override;
	virtual void BeforeDestroy() override;
};
