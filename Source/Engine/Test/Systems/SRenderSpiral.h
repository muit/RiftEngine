// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "ECS/ECSManager.h"

#include "Core/Assets/AssetPtr.h"
#include "Assets/Material.h"


class SRenderSpiral : public System {
	CLASS(SRenderSpiral, System)

	TAssetPtr<Material> material {"Test/spiral.shader.meta"};

public:

	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;
};
