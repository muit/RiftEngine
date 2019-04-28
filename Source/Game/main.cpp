// Copyright 2015-2019 Piperift - All rights reserved

#include <cstdio>
#include <iostream>

#include <Core/Engine.h>
#include <Core/Events/Broadcast.h>


int main(int, char**)
{
	const bool bSuccess = Engine::StartEngine();
	return !bSuccess;
}
