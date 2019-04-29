// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include <EASTL/type_traits.h>

#include "AssetInfo.h"
#include "AssetData.h"
#include "AssetManager.h"
//#include "Core/Serialization/Archive.h"
#include "../TypeTraits.h"


template<class T>
class TAssetPtr
{
	static_assert(eastl::is_base_of<AssetData, T>::value, "AssetPtr type must inherit from AssetData");

	AssetInfo info;
	mutable Ptr<T> cachedAsset;


public:

	using ItemType = T;

	TAssetPtr() : info{}, cachedAsset{} {}

	TAssetPtr(TAssetPtr&& other) { MoveFrom(MoveTemp(other)); }
	TAssetPtr(const TAssetPtr& other) { CopyFrom(other); }

	TAssetPtr(Name path) : info{ path } {}
	TAssetPtr(const TCHAR* key) : TAssetPtr(Name{ key }) {}
	TAssetPtr(const AssetInfo& other) : AssetInfo(other) {}
	TAssetPtr(Ptr<ItemType> asset) {
		if (asset)
		{
			info = asset->GetInfo();
			cachedAsset = asset;
		}
	}

	TAssetPtr& operator=(TAssetPtr&& other)      { MoveFrom(MoveTemp(other)); return *this; }
	TAssetPtr& operator=(const TAssetPtr& other) { CopyFrom(other); return *this; }

	/**
	 * Tries to load this asset if it's not already
	 * @returns the loaded asset
	 */
	const Ptr<T>& Load() const
	{
		if(IsNull() || IsValid())
			return cachedAsset;

		if (auto manager = AssetManager::Get())
		{
			cachedAsset = manager->Load(info).Cast<T>();
		}
		return cachedAsset;
	}

	/**
	 * Tries to load this asset and returns a new one if not found
	 * @returns the loaded asset
	 */
	const Ptr<T>& LoadOrCreate() const
	{
		if (IsNull() || IsValid())
			return cachedAsset;

		if (auto manager = AssetManager::Get())
		{
			cachedAsset = manager->LoadOrCreate(info, T::StaticClass()).Cast<T>();
		}
		return cachedAsset;
	}

	/**
	 * @returns the asset if it's valid and loaded
	 */
	const Ptr<T>& Get() const
	{
		if(IsNull())
		{
			return cachedAsset; // Cached asset should always be invalid here
		}

		Ptr<AssetManager> manager = AssetManager::Get();
		if (manager && !cachedAsset)
		{
			cachedAsset = manager->GetLoadedAsset(info).Cast<T>();
		}

		return cachedAsset;
	}

	void Reset()
	{
		info = {};
		cachedAsset = nullptr;
	}

	/** @returns true if this can never be pointed towards an asset */
	const bool IsNull() const { return info.IsNull(); }

	/** @returns true if this asset is loaded */
	const bool IsValid() const { return Get().IsValid(); }

	AssetInfo GetInfo() { return info; }

	inline const Name& GetPath()    const { return info.GetPath(); }
	inline const String& GetSPath() const { return info.GetSPath(); }

	operator bool() const { return IsValid(); };
	Ptr<T> operator*()  const { return Get(); }
	Ptr<T> operator->() const { return Get(); }

	FORCEINLINE bool Serialize(class Archive& ar, const char* inName)
	{
		return info.Serialize(ar, inName);
	}

private:

	void MoveFrom(TAssetPtr&& other)
	{
		info = other.info;
		cachedAsset = other.cachedAsset;
		other.info = {};
		other.cachedAsset = nullptr;
	}

	void CopyFrom(const TAssetPtr& other)
	{
		info = other.info;
		cachedAsset = other.cachedAsset;
	}
};

DEFINE_TEMPLATE_CLASS_TRAITS(TAssetPtr,
	HasCustomSerialize = true
);
