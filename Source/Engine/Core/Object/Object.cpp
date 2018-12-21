// Copyright 2015-2019 Piperift - All rights reserved

#include "Object.h"
#include "Engine.h"

Ptr<World> Object::GetWorld() const
{
	return GEngine->GetWorld();
}
