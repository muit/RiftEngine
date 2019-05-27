// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "Core/Assets/AssetPtr.h"
#include "Assets/Material.h"


class SRenderMeshes : public System {
	CLASS(SRenderMeshes, System)

	static const TAssetPtr<Material> skyboxMaterial;

public:

	SRenderMeshes();

	virtual void Tick(float /*deltaTime*/) override;
	virtual void BeforeDestroy() override;

private:

	void DrawSkybox();
};
