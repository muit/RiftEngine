// Copyright 2015-2019 Piperift - All rights reserved

#include <cstdio>
#include <iostream>
#include "Core/Strings/Name.h"

#include <Core/Engine.h>


int main(int, char**)
{
	const bool bSuccess = Engine::StartEngine();
	return !bSuccess;
}
