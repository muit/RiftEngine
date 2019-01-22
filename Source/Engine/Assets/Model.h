// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreObject.h"
#include "Core/Assets/AssetData.h"
#include "Rendering/MeshData.h"


class Model : public AssetData
{
	CLASS(Model, AssetData)


	MeshData data;


public:

	virtual bool PostLoad() override;

	FORCEINLINE const MeshData& GetMeshData() const { return data; }
};
