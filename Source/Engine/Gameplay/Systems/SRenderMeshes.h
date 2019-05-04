// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "Core/Assets/AssetPtr.h"
#include "Assets/Texture.h"


class SRenderMeshes : public System {
	CLASS(SRenderMeshes, System)

	TAssetPtr<Texture> texture;


public:

	SRenderMeshes() : Super(), texture{ "test.png.meta" } { bTickOnEditor = true; }

	virtual void BeginPlay() override;
	virtual void Tick(float /*deltaTime*/) override;
	virtual void BeforeDestroy() override;
};
