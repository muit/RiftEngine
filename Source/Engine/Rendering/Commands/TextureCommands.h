// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Tools/Profiler.h"

#include "Core/MultiThreading.h"
#include "Core/Assets/AssetPtr.h"
#include "Assets/Texture.h"
#include "RenderCommand.h"
#include "../Renderer.h"
#include "../Frame.h"


class LoadTextureCommand : public RenderCommand {
public:
	TAssetPtr<Texture> asset;


	LoadTextureCommand(TAssetPtr<Texture> asset) : asset{ asset } {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		ScopedGraphicsZone("Load Texture Command");
		// #TODO: Ensure assets stay loaded during this frame
		render.resources.Load(asset.GetPath(), asset->GetTextureData());
	}
};

class FreeTextureCommand : public RenderCommand {
public:
	AssetInfo asset;


	FreeTextureCommand(AssetInfo asset) : asset{ asset } {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		ScopedGraphicsZone("Free Texture Command");
		render.resources.Free<ResourceType::Texture>(asset.GetPath());
	}
};
