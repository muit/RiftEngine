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
	info = { path };
	return Save();
}

bool AssetData::Save()
{
	if (info.GetPath().IsNone())
		return false;

	JsonArchive ar {};
	ar("asset_type", GetClass()->GetNameRef());
	Serialize(ar);

	return FileSystem::SaveJsonFile(info.GetPath().ToString(), ar.GetData());
}
