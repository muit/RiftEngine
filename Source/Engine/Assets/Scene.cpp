// Copyright 2015-2019 Piperift - All rights reserved

#include "Scene.h"
#include "Core/World.h"
#include "Core/Serialization/Archive.h"


bool Scene::Serialize(Archive& ar)
{
	bool bSuccess = Super::Serialize(ar);

	if (bSuccess)
	{
		Ptr<World> world = GetWorld();
		if (world)
		{
			world->GetECS()->Serialize(ar);
		}
	}
	return bSuccess;
}
