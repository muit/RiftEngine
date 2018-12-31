// Copyright 2015-2019 Piperift - All rights reserved

#include "FileSystem.h"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


bool FileSystem::FileExists(const String& path)
{
	const fs::path pathObj{ path.begin(), path.end() };
	return fs::exists(pathObj) && fs::is_regular_file(pathObj);
}

bool FileSystem::FolderExists(const String& path)
{
	const fs::path pathObj{ path.begin(), path.end() };
	return fs::exists(pathObj) && fs::is_directory(pathObj);
}

String FileSystem::GetContentPath()
{
	const fs::path path = fs::current_path();
	// #TODO: Maybe add content folder
	const std::string pathStr = path.string();
	return String{ pathStr.c_str(), pathStr.size() };
}
