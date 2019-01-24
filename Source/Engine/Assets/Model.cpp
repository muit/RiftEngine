// © 2019 Miguel Fernández Arce - All rights reserved

#include "Model.h"

#include "Core/World.h"
#include "Core/Assets/AssetInfo.h"
#include "Rendering/Commands/ResourceCommands.h"


bool Model::PostLoad()
{
	Super::PostLoad();

	data = {};

	Path rawModelPath = FileSystem::FindRawFile(FileSystem::FromString(Info().GetSPath()));
	if (!rawModelPath.empty())
	{
		//SDL_Surface* rawImg = IMG_Load(FileSystem::ToString(rawTexturePath).c_str());
		// Copy texture data
		//data.FromSurface(rawImg);

		GetWorld()->QueueRender<LoadMeshCommand>(0, data);
		return true;
	}

	return false;
}
