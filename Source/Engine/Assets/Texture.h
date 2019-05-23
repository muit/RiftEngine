// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreObject.h"
#include "Core/Assets/AssetData.h"
#include "Core/Math/Color.h"
#include "Core/Assets/AssetPtr.h"
#include "Rendering/Data/TextureData.h"


class Texture : public AssetData
{
	CLASS(Texture, AssetData)

private:

	TextureData data;

public:

	static const TAssetPtr<Texture> default;


	Texture() : Super() {}

	virtual bool PostLoad() override;
	virtual void OnUnload() override;

	FORCEINLINE bool IsEmpty() const { return data.Size().x == 0 && data.Size().y == 0; }
	const v2_u32& GetSize() const;
	const TextureData& GetTextureData() const;
};
