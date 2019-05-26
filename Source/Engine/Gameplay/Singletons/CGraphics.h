// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/CSingleton.h"
#include "Assets/CubeTexture.h"


class CGraphics : public CSingleton {
	STRUCT(CGraphics, CSingleton)

public:

	PROP(Color, ambientColor, DetailsEdit)
	Color ambientColor = Color::White;

	PROP(float, ambientIntensity, DetailsEdit)
	float ambientIntensity = 0.3f;

	PROP(TAssetPtr<CubeTexture>, cubeMap, DetailsEdit)
	TAssetPtr<CubeTexture> cubeMap;
};
