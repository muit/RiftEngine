// © 2019 Miguel Fernández Arce - All rights reserved

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

	virtual bool PostLoad() { return true; }


	/** HELPERS */
public:

	const AssetInfo& Info() const { return info; }

	const Name& GetMetaPath() const { return info.GetPath(); }
	const Name& GetRawPath() const { return info.GetPath(); }
};
