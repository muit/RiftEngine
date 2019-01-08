// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreObject.h"
#include "Core/Assets/AssetData.h"
#include "Core/Math/Color.h"
#include "Core/Rendering/TextureData.h"


class Texture : public AssetData
{
	CLASS(Texture, AssetData)

public:

	Texture() : Super() {}

private:

	TextureData data;

protected:

	virtual bool PostLoad() override;
	virtual void BeforeDestroy() override;

	void Render();

public:

	FORCEINLINE bool IsEmpty() const { return data.size.x() == 0 && data.size.y() == 0; }
	FORCEINLINE v2_u32 GetSize() const { return data.size; }
};
