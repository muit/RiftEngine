// Copyright 2017/2018 - Miguel Fernández Arce

#pragma once

#include "CoreObject.h"
#include "Core/Assets/AssetData.h"


class Texture : public AssetData
{
	CLASS(Texture, AssetData)

public:

	PROP(String, texturePath, DetailsEdit)
	String texturePath;


	virtual bool PostLoad() override;
};
