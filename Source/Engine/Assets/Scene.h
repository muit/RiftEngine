// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "Core/Assets/AssetData.h"
#include "Core/Strings/String.h"
#include "Core/Strings/Name.h"
#include "Core/Math/Vector.h"
#include "Core/Files/FileSystem.h"


class Scene : public AssetData {
	CLASS(Scene, AssetData)

public:

	PROP(String, displayName, DetailsEdit)
	String displayName;

	PROP(Json, data)
	Json data;


	virtual bool Serialize(Archive& ar) override;

	bool ApplyScene(const Ptr<World>& world);
	bool SaveScene(const Ptr<World>& world);
};
