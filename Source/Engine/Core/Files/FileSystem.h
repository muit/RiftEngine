// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "EngineTypes.h"
#include "Core/Serialization/Archive.h"
#include <filesystem>

namespace fs = std::filesystem;

class FileSystem
{
public:

	static bool FileExists(const String& path);
	static bool FolderExists(const String& path);

	static bool LoadJsonFile(const String& path, json& result);
	static bool SaveJsonFile(const String& path, const json& data);

	static String GetAssetsPath() {
		const fs::path path = GetAssetsPathAsPath();

		const std::string pathStr = path.string();
		return String{ pathStr.c_str(), pathStr.size() };
	}

private:

	static inline bool SanitizeAssetPath(fs::path& path) {
		return path.has_filename() && SanitizeDirectoryPath(path);
	}

	static inline bool SanitizeDirectoryPath(fs::path& path) {
		if (path.is_relative())
			path = GetAssetsPathAsPath() / path;
		return true;
	}

	static fs::path GetAssetsPathAsPath();


	static fs::path FindMetaFile(fs::path in);
	static fs::path FindRawFile(fs::path in);
};
