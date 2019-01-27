// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "ECS/ECSManager.h"
#include "Core/Assets/AssetPtr.h"

#include "Assets/Model.h"


class CMesh : public Component {
	STRUCT(CMesh, Component)

	PROP(TAssetPtr<Model>, model)
	TAssetPtr<Model> model;


	CMesh() : Super(), model{} {}
	CMesh(TAssetPtr<Model> model) : Super(), model{ model } {
		model.LoadOrCreate();
	}

	void PostSerialize(bool/* bLoaded*/) {
		model.Load();
	}
};

DEFINE_CLASS_TRAITS(CMesh,
	HasPostSerialize = true
);