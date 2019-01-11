// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "Core/Assets/AssetPtr.h"
#include "Assets/Texture.h"


class SRenderMesh : public System {
	CLASS(SRenderMesh, System)

	TAssetPtr<Texture> texture;


public:

	SRenderMesh() : Super(), texture{ "test.png.meta" } {}

	virtual void BeginPlay() override;
	virtual void Tick(float /*deltaTime*/) override;
	virtual void BeforeDestroy() override;
};
