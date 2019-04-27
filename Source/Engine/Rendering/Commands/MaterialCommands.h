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
	AssetInfo asset;
	MaterialData data;


	LoadMaterialCommand(AssetInfo asset, MaterialData data) : asset{ asset }, data{data} {}

	virtual void Execute(FrameRender& render, Frame& frame) override
	{
		render.resources.Load(asset.GetPath(), MoveTemp(data));
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
