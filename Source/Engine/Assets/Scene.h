// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "Core/Assets/AssetData.h"
#include "Core/Strings/String.h"
#include "Core/Strings/Name.h"
#include "Core/Math/Vector.h"


class Scene : public AssetData {
	CLASS(Scene, AssetData)

public:

	PROP(String, displayName, DetailsEdit)
	String displayName;


	virtual bool Serialize(class Archive& ar) override;
};
