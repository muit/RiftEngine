// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "ECS/ECSManager.h"
#include "Core/Assets/AssetPtr.h"

#include "Assets/Model.h"


class CMesh : public Component {
	STRUCT(CMesh, Component)

	TAssetPtr<Model> model;

	CMesh() : Super(), model{} {}
	CMesh(TAssetPtr<Model> model) : Super(), model{model} {
		model.LoadOrCreate();
	}
};
