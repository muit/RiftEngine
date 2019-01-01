// Copyright 2017/2018 - Miguel Fernández Arce

#pragma once

#include "CoreObject.h"
#include "AssetInfo.h"


class AssetData : public Object
{
	CLASS(AssetData, Object)

	AssetInfo info;

public:

	AssetData() : Super() {}

	/** Deserializes the asset. Internal usage only */
	bool __Load(const AssetInfo& inInfo, json& data);


	bool SaveToPath(Name path);

	bool Save();

protected:

	virtual bool PostLoad()
	{ return true; }
};
