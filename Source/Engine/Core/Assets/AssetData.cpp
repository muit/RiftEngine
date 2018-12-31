// Copyright 2017/2018 - Miguel Fernández Arce

#include "AssetData.h"
#include "Core/Serialization/Archive.h"
#include "Core/Files/FileSystem.h"


bool AssetData::__Load(const AssetInfo& inInfo, json& data)
{
	info = inInfo;

	// Deserialize asset
	JsonArchive ar(data, true);
	Serialize(ar);

	return PostLoad();
}

bool AssetData::Save()
{
	if (info.GetPath().IsNone())
		return false;

	json data;
	JsonArchive ar(data, false);
	Serialize(ar);

	return FileSystem::SaveJsonFile(info.GetPath().ToString(), data);
}
