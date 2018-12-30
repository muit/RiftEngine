// Copyright 2017/2018 - Miguel Fernández Arce

#pragma once

#include "CoreObject.h"


class AssetInfo;

class AssetData : public Object
{
	CLASS(AssetData, Object)

protected:

	AssetData() {}

public:

	//Called when loading the asset
	AssetData(const AssetInfo& info) {}

	virtual bool Construct(const AssetInfo& info)
	{ return true; }
};
