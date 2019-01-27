// Copyright 2015-2019 Piperift - All rights reserved

#include "AssetManager.h"

#include "Core/Files/FileSystem.h"
#include "Core/Object/ObjectPtr.h"
#include "Core/World.h"
#include "Core/Engine.h"


Ptr<AssetData> AssetManager::Load(const AssetInfo& info)
{
	if (info.IsNull() || !FileSystem::IsAssetPath(info.GetSPath()))
		return {};

	json data;
	if (FileSystem::LoadJsonFile(info.GetPath().ToString(), data))
	{
		const auto type{ data["asset_type"] };
		if (!type.is_string())
			return {}; // Asset doesn't have a type

		// Get asset type from json
		Class* assetClass = AssetData::StaticClass()->FindChildClass(Name{ type });
		if (!assetClass)
			return {}; // Asset doesn't have a valid class

		// Create asset from json type
		GlobalPtr<AssetData> newAsset = assetClass->CreateInstance(GetSelf()).Cast<AssetData>();

		if (newAsset->OnLoad(info, data))
		{
			const Ptr<AssetData> newAssetPtr = newAsset;

			// Loading succeeded, registry the asset
			loadedAssets[info.GetPath()] = eastl::move(newAsset);
			return eastl::move(newAssetPtr);
		}
	}
	return {};
}

Ptr<AssetData> AssetManager::LoadOrCreate(const AssetInfo& info, Class* assetType)
{
	if (info.IsNull() || !FileSystem::IsAssetPath(info.GetSPath()))
		return {};

	Ptr<AssetData> loadedAsset = Load(info);
	if(loadedAsset)
	{
		return loadedAsset;
	}

	if (!assetType)
		return {};

	if (assetType->IsChildOf<AssetData>())
	{
		GlobalPtr<AssetData> newAsset = assetType->CreateInstance(GetSelf()).Cast<AssetData>();
		if (newAsset->OnCreate(info))
		{
			const Ptr<AssetData> newAssetPtr = newAsset;

			loadedAssets[info.GetPath()] = eastl::move(newAsset);

			return newAssetPtr;
		}
	}
	return {};
}


Ptr<AssetManager> AssetManager::Get(Ptr<Object> context)
{
	if (!context)
		context = GEngine;

	if (Ptr<World> world = context->GetWorld()) {
		return world->GetAssetManager();
	}
	return {};
}
