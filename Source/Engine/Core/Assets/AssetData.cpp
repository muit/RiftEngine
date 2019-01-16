// © 2019 Miguel Fernández Arce - All rights reserved

#include "AssetData.h"
#include "Core/Serialization/Archive.h"
#include "Core/Files/FileSystem.h"


bool AssetData::__Load(const AssetInfo& inInfo, json& data)
{
	info = inInfo;

	// Deserialize asset
	JsonArchive ar(data);
	Serialize(ar);

	return PostLoad();
}

bool AssetData::SaveToPath(Name path)
{
	// #FIX: SaveToPath should modify the current asset
	info = { path };
	return Save();
}

bool AssetData::Save()
{
	if (info.GetPath().IsNone())
		return false;

	JsonArchive ar {};
	Name className = GetClass()->GetName();
	ar("asset_type", className);
	Serialize(ar);

	return FileSystem::SaveJsonFile(info.GetPath().ToString(), ar.GetData());
}
