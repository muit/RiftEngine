// Copyright 2017/2018 - Miguel Fernández Arce

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

bool AssetData::Save()
{
	if (info.GetPath().IsNone())
		return false;

	JsonArchive ar {};
	ar("asset_type", GetClass()->GetNameRef());
	Serialize(ar);

	return FileSystem::SaveJsonFile(info.GetPath().ToString(), ar.GetData());
}
