// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "EngineTypes.h"
#include "Core/Serialization/Archive.h"
#include <filesystem>

namespace fs = std::filesystem;

using Path = fs::path;

class FileSystem
{
public:
	/** String API */

	static FORCEINLINE bool FileExists(const String& path)                     { return FileExists  (FromString(path));         }
	static FORCEINLINE bool FolderExists(const String& path)                   { return FolderExists(FromString(path));         }
	static FORCEINLINE bool LoadJsonFile(const String& path, json& result)     { return LoadJsonFile(FromString(path), result); }
	static FORCEINLINE bool SaveJsonFile(const String& path, const json& data) { return SaveJsonFile(FromString(path), data);   }

	static FORCEINLINE String GetAssetsPath() { return ToString(GetAssetsPathAsPath()); }


	/** Path API */

	static bool FileExists(const Path& path);
	static bool FolderExists(const Path& path);


	static bool LoadJsonFile(Path path, json& result);
	static bool SaveJsonFile(Path path, const json& data);

	static Path GetAssetsPathAsPath();

	static Path FindMetaFile(Path in);

	static Path FindRawFile(Path in);


	/** HELPERS */

	static String ToString(const Path& path) {
		const std::string pathStr = path.string();
		return String{ pathStr.c_str(), pathStr.size() };
	}

	static Path FromString(const String& path) {
		return { path.begin(), path.end() };
	}

private:

	static inline bool SanitizeAssetPath(Path& path) {
		return path.has_filename() && SanitizeDirectoryPath(path);
	}

	static inline bool SanitizeDirectoryPath(Path& path) {

		if (path.is_relative())
		{
			path = GetAssetsPathAsPath() / path;
			return true;
		}

		// Ensure path is inside Assets folder
		return !fs::relative(path, GetAssetsPathAsPath()).empty();

	}
};
