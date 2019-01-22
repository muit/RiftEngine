// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreObject.h"
#include "Core/Assets/AssetData.h"
#include "Core/Math/Color.h"
#include "Rendering/TextureData.h"


class Texture : public AssetData
{
	CLASS(Texture, AssetData)


private:

	TextureData data;

public:

	Texture() : Super() {}

	virtual bool PostLoad() override;

	FORCEINLINE bool IsEmpty() const { return data.Size().x() == 0 && data.Size().y() == 0; }
	FORCEINLINE const v2_u32& GetSize() const { return data.Size(); }
	FORCEINLINE const TextureData& GetTextureData() const { return data; }
};
