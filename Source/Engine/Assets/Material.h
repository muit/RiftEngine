// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreObject.h"
#include "Core/Assets/AssetData.h"
#include "Core/Math/Color.h"
#include "Rendering/Data/TextureData.h"


class Material : public AssetData
{
	CLASS(Material, AssetData)

private:



public:

	Material() : Super() {}

	virtual bool PostLoad() override;
	virtual void OnUnload() override;
};
