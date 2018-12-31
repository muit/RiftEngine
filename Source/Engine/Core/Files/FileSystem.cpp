// Copyright 2015-2019 Piperift - All rights reserved

#include "FileSystem.h"


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

String FileSystem::GetAssetsPath()
{
	const fs::path path = GetAssetsAsPath();
	const std::string pathStr = path.string();
	return String{ pathStr.c_str(), pathStr.size() };
}

bool FileSystem::LoadJsonFile(const String& inPath, json& result)
{
	fs::path path { inPath.begin(), inPath.end() };

	if (!SanitizeAssetPath(path) || !fs::exists(path))
		return false;

	std::ifstream file(path);

	result = {};
	result << file;
	return true;
}

bool FileSystem::SaveJsonFile(const String& inPath, const json& data)
{
	fs::path path{ inPath.begin(), inPath.end() };

	if (!SanitizeAssetPath(path) || !fs::exists(path))
		return false;

	std::ofstream file(path);
	file << data;

	return true;
}

fs::path FileSystem::GetAssetsAsPath()
{
	// Take two folders up. Different for distributed versions / other platforms
	fs::path path = fs::current_path().parent_path().parent_path();
	path /= "Assets";
	return eastl::move(path);
}
