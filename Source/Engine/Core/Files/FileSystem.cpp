// Copyright 2015-2019 Piperift - All rights reserved

#include "FileSystem.h"


bool FileSystem::FileExists(const Path& path)
{
	return fs::exists(path) && fs::is_regular_file(path);
}

bool FileSystem::FolderExists(const Path& path)
{
	return fs::exists(path) && fs::is_directory(path);
}

bool FileSystem::LoadJsonFile(Path path, json& result)
{
	if (!SanitizeAssetPath(path) || !fs::exists(path))
		return false;

	std::ifstream file(path);

	result = {};
	result << file;
	return true;
}

bool FileSystem::SaveJsonFile(Path path, const json& data)
{
	if (!SanitizeAssetPath(path))
		return false;

	std::ofstream file(path);
	file << data;

	return true;
}


Path FileSystem::GetAssetsPath()
{
	// Take two folders up. May change for distributed versions / other platforms
	Path path = fs::current_path().parent_path().parent_path();
	path /= "Assets";
	return eastl::move(path);
}

Path FileSystem::FindMetaFile(Path in)
{
	if (!SanitizeAssetPath(in))
		return {};

	if (in.extension() != ".meta")
		in += ".meta";

	return in;
}

Path FileSystem::FindRawFile(Path in)
{
	if (!SanitizeAssetPath(in))
		return {};

	const Path stem = in.stem();
	if (!stem.has_extension() || in.extension() != ".meta")
		return {};

	return in.parent_path() / stem;
}
