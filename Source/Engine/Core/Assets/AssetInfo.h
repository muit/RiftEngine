// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreObject.h"
#include "Core/Files/FileSystem.h"


class AssetInfo : public Object
{
	CLASS(AssetInfo, Object)

public:
	AssetInfo() : id(Name::None()) {}
	AssetInfo(Name id) : id(id) {}

protected:

	Name id;

public:

	/**
	 * @returns true if this can never be pointed towards an asset
	 */
	const bool IsNull() const {
		const String& path = GetSPath();
		return path.empty() || !FileSystem::IsAssetPath(path);
	}

	inline const Name& GetPath()    const { return id; }
	inline const String& GetSPath() const { return id.ToString(); }
};
