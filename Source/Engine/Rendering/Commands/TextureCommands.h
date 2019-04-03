// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

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
		// #TODO: Ensure assets stay loaded during this frame
		render.resources.Load(asset.GetPath(), asset->GetTextureData());
	}
};

class FreeTextureCommand : public RenderCommand {
public:
	AssetInfo asset;


	FreeTextureCommand(AssetInfo asset) : asset{ asset } {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		render.resources.Free<ResourceType::Texture>(asset);
	}
};

class Draw2DTextureCommand : public RenderCommand {
public:
	AssetInfo asset;
	v2_u32 position;


	Draw2DTextureCommand(AssetInfo asset, v2_u32 position)
		: asset{asset}, position{position}
	{}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		//u32 texture = render.resources.GetGLId<ResourceType::Texture>(id);

		//render.DrawImage(position, texture);
	}
};
