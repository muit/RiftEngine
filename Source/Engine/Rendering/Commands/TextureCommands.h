// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include "Core/MultiThreading.h"
#include "Core/Assets/AssetPtr.h"
#include "Assets/Texture.h"
#include "Assets/Mesh.h"
#include "RenderCommand.h"
#include "../Renderer.h"
#include "../Frame.h"


class LoadTextureCommand : public RenderCommand {
public:
	TAssetPtr<Texture> asset;


	LoadTextureCommand(TAssetPtr<Texture> asset) : asset{ asset } {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		// #TODO: Avoid texture data copy by adding dependency on the asset
		//render.resources.Load(id, texture);
	}
};

class FreeTextureCommand : public RenderCommand {
public:
	AssetInfo asset;


	FreeTextureCommand(AssetInfo asset) : asset{ asset } {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		//render.resources.Free<ResourceType::Texture>(asset);
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
