// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "EngineTypes.h"


class FileSystem
{
public:

	static bool FileExists(const String& path);
	static bool FolderExists(const String& path);

	static String GetContentPath();
};
