// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "EngineTypes.h"
#include "Core/Serialization/Archive.h"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


class FileSystem
{
public:

	static bool FileExists(const String& path);
	static bool FolderExists(const String& path);

	static String GetAssetsPath();

	static bool LoadJsonFile(const String& path, json& result);
	static bool SaveJsonFile(const String& path, const json& data);

private:

	static inline bool SanitizeAssetPath(fs::path& path) {
		return path.has_filename() && SanitizeDirectoryPath(path);
	}

	static inline bool SanitizeDirectoryPath(fs::path& path) {
		if (path.is_relative())
			path = GetAssetsAsPath() / path;
		return true;
	}

	static fs::path GetAssetsAsPath();
};
