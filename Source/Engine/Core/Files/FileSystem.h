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

private:

	static fs::path GetAssetsAsPath();
};
