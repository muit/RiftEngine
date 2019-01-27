// © 2019 Miguel Fernández Arce - All rights reserved

#include "AssetData.h"
#include "Core/Serialization/Archive.h"
#include "Core/Files/FileSystem.h"



bool AssetData::OnCreate(const AssetInfo& inInfo)
{
	info = inInfo;
	return PostLoad();
}

bool AssetData::OnLoad(const AssetInfo& inInfo, json& data)
{
	info = inInfo;

	// Deserialize asset
	JsonArchive ar(data);
	Serialize(ar);

	return PostLoad();
}

bool AssetData::SaveToPath(const Name& path)
{
	String spath = path.ToString();
	if (spath.empty() || !FileSystem::IsAssetPath(spath))
		return false;

	JsonArchive ar{};
	Name className = GetClass()->GetName();
	ar("asset_type", className);
	Serialize(ar);

	return FileSystem::SaveJsonFile(spath, ar.GetData(), ar.GetIndent());
}

bool AssetData::Save()
{
	return SaveToPath(info.GetPath());
}
