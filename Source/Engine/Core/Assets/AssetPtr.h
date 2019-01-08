// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include <EASTL/type_traits.h>

#include "AssetInfo.h"
#include "AssetData.h"
#include "AssetManager.h"


template<class T>
class TAssetPtr : public AssetInfo
{
	static_assert(eastl::is_base_of<AssetData, T>::value, "AssetPtr type must inherit from AssetData");

private:

	mutable Ptr<T> cachedAsset;

public:
	TAssetPtr() : AssetInfo(), cachedAsset{} {}
	TAssetPtr(Name id)
		: AssetInfo(id)
		, cachedAsset{}
	{}
	TAssetPtr(Name id, String inPath)
		: AssetInfo(id, inPath)
		, cachedAsset{}
	{}

	/**
		* @returns true if this asset is loaded
		*/
	const bool IsValid() const {
		return Get().IsValid();
	}

	/**
		* Tries to load this asset if it's not already
		* @returns the loaded asset
		*/
	const Ptr<T>& Load() const {
		if(IsNull())
			return cachedAsset; // Cached asset should always be invalid here

		auto manager = AssetManager::Get();
		if(manager && !IsValid())
			cachedAsset = manager->Load(*this).Cast<T>();

		return cachedAsset;
	}

	/**
	 * @returns the asset if it's valid and loaded
	 */
	const Ptr<T>& Get() const {
		if(IsNull())
			return cachedAsset; // Cached asset should always be invalid here

		auto manager = AssetManager::Get();
		if(manager && !cachedAsset)
			cachedAsset = manager->GetLoadedAsset(id).Cast<T>();

		return cachedAsset;
	}


	operator bool() const { return IsValid(); };
	Ptr<T> operator*()  const { return Get(); }
	Ptr<T> operator->() const { return Get(); }
};
