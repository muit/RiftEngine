// Copyright 2015-2019 Piperift - All rights reserved

#include <cstdio>
#include <iostream>

#include <EngineTypes.h>
#include <Engine.h>
#include "World/World.h"
#include "Gameplay/Actor.h"


using namespace std;

int main(int, char**)
{
	Engine engine{};
	engine.Start();

	auto a = GlobalPtr<Actor>();
	Ptr<Object> b = a.GetPtr();
	auto c = a.GetPtr();

	if (a == c)
	{
		cout << "true";
	} else {
		cout << "false";
	}

	return 0;
}
