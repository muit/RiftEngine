// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "Core/Object/ObjectPtr.h"

#include "AssetPtr.h"
#include "AssetData.h"


class AssetManager : public Object {
	CLASS(AssetManager, Object)

private:

	TArray<AssetInfo> assetInfos;

	eastl::unordered_map<Name, GlobalPtr<AssetData>> loadedAssets;


	void Internal_StaticLoad(const AssetInfo& asset);
	void Internal_StaticLoad(const TArray<AssetInfo>& assets);

public:

	/*void Initialize() {}

	void Shutdown() {}*/

	template<class T>
	Ptr<T> TryLoad(const AssetInfo& info) {
		static_assert(eastl::is_base_of<AssetData, T>::value, "AssetPtr type must inherit from AssetData");

		if (info.IsNull() || info.GetPath().empty())
			return nullptr;

		GlobalPtr<T> newAsset = Create<T>(info);
		if (newAsset->Construct(info))
		{
			const Ptr<T> newAssetptr = newAsset;
			// Loading succeeded, registry the asset
			loadedAssets[info.GetId()] = eastl::move(newAsset);
			return eastl::move(newAssetPtr);
		}
		return nullptr;
	}

	inline const Ptr<AssetData>& GetAssetById(AssetInfo id) {
		return loadedAssets[id.GetPath()];
	}
	template<class T>
	inline Ptr<T> GetAssetById(AssetInfo id) {
		static_assert(is_base_of<AssetData, T>::value, "Template must inherit from AssetData");
		return static_pointer_cast<T>(loadedAssets[id]);
	}


	/** STATIC */

private:

	static const GlobalPtr<AssetManager> globalManager;

public:

	static Ptr<AssetManager> Get() { return globalManager; }
};
