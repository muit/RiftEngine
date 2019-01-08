// Copyright 2017/2018 - Miguel Fernández Arce

#pragma once

#include "CoreObject.h"
#include "Core/Assets/AssetData.h"
#include "Core/Math/Color.h"

using PixelBuffer = TArray<Color>;


class Texture : public AssetData
{
	CLASS(Texture, AssetData)

public:

	PROP(v2_u32, size, DetailsView)
	v2_u32 size;

	PROP(PixelBuffer, buffer)
	PixelBuffer buffer;


	virtual bool PostLoad() override;

	void Render();
};
