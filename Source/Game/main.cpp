// Copyright 2015-2019 Piperift - All rights reserved

#include <cstdio>
#include <iostream>

#include <Core/Engine.h>
#include <Core/Strings/Name.h>

#include "Core/Containers/Map.h"


int main(int, char**)
{
	TMap<u8, u32> d;

	const bool bSuccess = Engine::StartEngine();
	return !bSuccess;
}
