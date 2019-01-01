// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "Core/Object/ObjectPtr.h"

#include "AssetData.h"


class AssetManager : public Object {
	CLASS(AssetManager, Object)

private:

	TArray<AssetInfo> assetInfos;

	eastl::unordered_map<Name, GlobalPtr<AssetData>> loadedAssets;


	void __StaticLoad(const AssetInfo& asset);
	void __StaticLoad(const TArray<AssetInfo>& assets);

public:

	/*void Initialize() {}

	void Shutdown() {}*/

	Ptr<AssetData> Load(const AssetInfo& info);

	inline Ptr<AssetData> GetLoadedAsset(const AssetInfo& id) {
		return loadedAssets[id.GetPath()];
	}


	static Ptr<AssetManager> Get(Ptr<Object> context = {});
};
