// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "ECS/ECSManager.h"
#include "Core/Assets/AssetPtr.h"

#include "Assets/Mesh.h"


class CMesh : public Component {
	STRUCT(CMesh, Component)

	PROP(TAssetPtr<Mesh>, model, DetailsEdit)
	TAssetPtr<Mesh> model;

	PROP(Color, color)
	Color color;


	CMesh() : Super(), model{} {}
	CMesh(TAssetPtr<Mesh> model) : Super(), model{ model } {
		model.LoadOrCreate();
	}

	void PostSerialize(bool/* bLoaded*/);
};

DEFINE_CLASS_TRAITS(CMesh,
	HasPostSerialize = true
);