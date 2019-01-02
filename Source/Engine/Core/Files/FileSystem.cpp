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

	if (!SanitizeAssetPath(path))
		return false;

	std::ofstream file(path);
	file << data;

	return true;
}


fs::path FileSystem::GetAssetsPathAsPath()
{
	// Take two folders up. May change for distributed versions / other platforms
	fs::path path = fs::current_path().parent_path().parent_path();
	path /= "Assets";
	return eastl::move(path);
}

fs::path FileSystem::FindMetaFile(fs::path in)
{
	return in;
}

fs::path FileSystem::FindRawFile(fs::path in)
{
	return in;
}
