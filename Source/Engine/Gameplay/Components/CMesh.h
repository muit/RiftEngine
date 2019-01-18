// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "ECS/ECSManager.h"


class CMesh : public Component {
	STRUCT(CMesh, Component)

	//PROP(AssetPtr<Mesh>, mesh)
	//AssetPtr<Mesh> mesh;
};
