// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "RenderCommand.h"

#include "Core/MultiThreading.h"
#include "Core/Assets/AssetPtr.h"
#include "Assets/Material.h"
#include "../Renderer.h"
#include "../Frame.h"


class LoadMaterialCommand : public RenderCommand {
public:
	TAssetPtr<Material> asset;


	LoadMaterialCommand(TAssetPtr<Material> asset) : asset{ asset } {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		// #TODO: Ensure assets stay loaded during this frame
		render.resources.Load(asset.GetPath(), asset->GetData());
	}
};

class FreeMaterialCommand : public RenderCommand {
public:
	AssetInfo asset;


	FreeMaterialCommand(AssetInfo asset) : asset{ asset } {}

	virtual void Execute(FrameRender& render, Frame& frame) override
	{
		render.resources.Free<ResourceType::Material>(asset.GetPath());
	}
};
