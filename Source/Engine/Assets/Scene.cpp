// Copyright 2015-2019 Piperift - All rights reserved

#include "Scene.h"
#include "World.h"
#include "Core/Serialization/Archive.h"


bool Scene::Serialize(Archive& ar)
{
	bool bResult = Super::Serialize(ar);
	ar.Serialize("scene", data);
	return bResult;
}

bool Scene::ApplyScene(const Ptr<World>& world)
{
	JsonArchive ar{ data };
	ar.context = *world;
	return world->Serialize(ar);
}

bool Scene::SaveScene(const Ptr<World>& world)
{
	JsonArchive ar{ false };
	ar.context = *world;
	const bool bResult = world->Serialize(ar);
	data = ar.GetData();

	return bResult;
}
