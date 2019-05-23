// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "Assets/CubeTexture.h"


class CGraphics : public Component {
	STRUCT(CGraphics, Component)

public:

	PROP(TAssetPtr<CubeTexture>, cubeMap, DetailsEdit)
	TAssetPtr<CubeTexture> cubeMap;
};
