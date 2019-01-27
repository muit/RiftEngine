// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include <EASTL/type_traits.h>

#include "AssetInfo.h"
#include "AssetData.h"
#include "AssetManager.h"
#include "../Serialization/Archive.h"


template<class T>
class TAssetPtr : public AssetInfo
{
	static_assert(eastl::is_base_of<AssetData, T>::value, "AssetPtr type must inherit from AssetData");

	mutable Ptr<T> cachedAsset;


public:

	using ItemType = T;

	TAssetPtr() : AssetInfo(), cachedAsset{} {}
	TAssetPtr(Name id)
		: AssetInfo(id)
		, cachedAsset{}
	{}

	TAssetPtr(const AssetInfo& other)
		: AssetInfo(other)
		, cachedAsset{}
	{}

	TAssetPtr(const TAssetPtr& other)
		: AssetInfo(other)
		, cachedAsset{other.cachedAsset}
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
		if(IsNull() || IsValid())
			return cachedAsset;

		if (auto manager = AssetManager::Get())
		{
			cachedAsset = manager->Load(*this).Cast<T>();
		}
		return cachedAsset;
	}

	/**
	 * Tries to load this asset and returns a new one if not found
	 * @returns the loaded asset
	 */
	const Ptr<T>& LoadOrCreate() const {
		if (IsNull() || IsValid())
			return cachedAsset;

		if (auto manager = AssetManager::Get())
		{
			cachedAsset = manager->LoadOrCreate(*this, T::StaticClass()).Cast<T>();
		}
		return cachedAsset;
	}

	/**
	 * @returns the asset if it's valid and loaded
	 */
	const Ptr<T>& Get() const {
		if(IsNull())
			return cachedAsset; // Cached asset should always be invalid here

		Ptr<AssetManager> manager = AssetManager::Get();
		if(manager && !cachedAsset)
			cachedAsset = manager->GetLoadedAsset(id).Cast<T>();

		return cachedAsset;
	}

	operator bool() const { return IsValid(); };
	Ptr<T> operator*()  const { return Get(); }
	Ptr<T> operator->() const { return Get(); }

	FORCEINLINE bool Serialize(Archive& ar, const char* inName)
	{
		ar(inName, id);
	}
};

DEFINE_TEMPLATE_CLASS_TRAITS(TAssetPtr,
	HasCustomSerialize = true
);
