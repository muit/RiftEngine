// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreObject.h"
#include "Core/Assets/AssetData.h"
#include "Core/Math/Color.h"
#include "Rendering/Data/MaterialData.h"
//#include "Core/Assets/AssetPtr.h"


class Material : public AssetData
{
	CLASS(Material, AssetData)

private:

	MaterialData data;

	/*PROP(TAssetPtr<Texture>, baseColor, DetailsEdit)
	TAssetPtr<Texture> baseColor;

	PROP(TAssetPtr<Texture>, normal, DetailsEdit)
	TAssetPtr<Texture> normal;

	PROP(TAssetPtr<Texture>, roughness, DetailsEdit)
	TAssetPtr<Texture> roughness;

	PROP(TAssetPtr<Texture>, metallic, DetailsEdit)
	TAssetPtr<Texture> metallic;*/

public:

	Material() : Super() {}

	const MaterialData& GetData() const { return data; }

protected:

	virtual bool PostLoad() override;
	virtual void OnUnload() override;

private:

	void LoadTextures();
};
