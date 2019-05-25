// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/CSingleton.h"
#include "Assets/CubeTexture.h"


class CGraphics : public CSingleton {
	STRUCT(CGraphics, CSingleton)

public:

	PROP(TAssetPtr<CubeTexture>, cubeMap, DetailsEdit)
	TAssetPtr<CubeTexture> cubeMap;
};
