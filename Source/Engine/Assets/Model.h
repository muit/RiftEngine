// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreObject.h"
#include "Core/Assets/AssetData.h"
#include "Rendering/Data/MeshData.h"


class Model : public AssetData
{
	CLASS(Model, AssetData)

	static i32 idCounter;
	i32 id;

	MeshData data;


public:

	Model() : Super(), id{++idCounter} {}

	virtual bool PostLoad() override;

	FORCEINLINE i32 GetId() { return id; }
	FORCEINLINE const MeshData& GetMeshData() const { return data; }
};
