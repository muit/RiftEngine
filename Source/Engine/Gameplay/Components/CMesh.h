// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "ECS/ECSManager.h"
#include "Core/Assets/AssetPtr.h"

#include "Assets/Mesh.h"
#include "Assets/Material.h"


class CMesh : public Component {
	STRUCT(CMesh, Component)

	PROP(TAssetPtr<Mesh>, model, DetailsEdit)
	TAssetPtr<Mesh> model;

	PROP(TAssetPtr<Material>, overrideMaterial, DetailsEdit)
	TAssetPtr<Material> overrideMaterial;


	CMesh() : Super(), model{} {}
	CMesh(TAssetPtr<Mesh> model) : Super(), model{ model } {
		model.LoadOrCreate();
	}

	void PostSerialize(bool/* bLoaded*/);
};

DEFINE_CLASS_TRAITS(CMesh,
	HasPostSerialize = true
);