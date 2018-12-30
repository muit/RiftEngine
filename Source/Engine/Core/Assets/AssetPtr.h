// Copyright 2017/2018 - Miguel Fernández Arce

#pragma once

#include "CoreEngine.h"

#include <EASTL/type_traits.h>

#include "AssetManager.h"
#include "AssetInfo.h"
#include "AssetData.h"


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
		return Get() != nullptr;
	}

	/**
		* Tries to load this asset if it's not already
		* @returns the loaded asset
		*/
	const Ptr<T> Load() const {
		if(IsNull())
			return nullptr;

		if(!IsValid())
			cachedAsset = AssetManager::Get().TryLoad<T>(director, *this);

		return cachedAsset;
	}

	/**
	 * @returns the asset if it's valid and loaded
	 */
	Ptr<T> Get() const {
		if(IsNull())
			return nullptr;

		if(!cachedAsset)
			cachedAsset = AssetManager::Get().GetAssetById<T>(id);

		return cachedAsset;
	}
};
