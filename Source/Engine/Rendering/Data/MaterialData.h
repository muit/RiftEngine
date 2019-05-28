// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Core/Strings/String.h"
#include "Core/Assets/AssetInfo.h"


struct MaterialParam {
	Name name;
};

struct MaterialTextureParam : public MaterialParam {
	AssetInfo asset;
};


struct MaterialData
{
public:

	String vertexCode;
	String fragmentCode;

	// Static Parameters
	TArray<MaterialTextureParam> textureParams;


	MaterialData() = default;
	MaterialData(String vertexCode, String fragmentCode)
		: vertexCode{MoveTemp(vertexCode)}
		, fragmentCode{MoveTemp(fragmentCode)}
	{}

	bool Parse(const String& code);
};
