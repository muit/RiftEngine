// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreObject.h"
#include "Core/Assets/AssetData.h"
#include "Core/Math/Color.h"
#include "Rendering/Data/MaterialData.h"


class Material : public AssetData
{
	CLASS(Material, AssetData)

private:

	MaterialData data;

	// #TODO: Create Parameters based on shader code
	PROP(Color, paramColor)
	Color paramColor;


public:

	Material() : Super() {}

	const MaterialData& GetData() const { return data; }

protected:

	virtual bool PostLoad() override;
	virtual void OnUnload() override;
};
