// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreObject.h"
#include "Core/Assets/AssetPtr.h"
#include "Core/Assets/AssetData.h"
#include "Rendering/Data/MeshData.h"
#include "Material.h"


class Mesh : public AssetData
{
	CLASS(Mesh, AssetData)

private:

	MeshData data;

public:

	PROP(TAssetPtr<Material>, material)
	TAssetPtr<Material> material;


	Mesh() : Super() {}

	virtual bool PostLoad() override;
	virtual void OnUnload() override;

	FORCEINLINE const MeshData& GetMeshData() const { return data; }

private:


};
