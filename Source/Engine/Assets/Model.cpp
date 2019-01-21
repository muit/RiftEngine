// © 2019 Miguel Fernández Arce - All rights reserved

#include "Model.h"

#include "Core/Assets/AssetInfo.h"



bool Model::PostLoad()
{
	Super::PostLoad();

	Path rawTexturePath = FileSystem::FindRawFile(FileSystem::FromString(Info().GetSPath()));
	if (!rawTexturePath.empty())
	{
		//SDL_Surface* rawImg = IMG_Load(FileSystem::ToString(rawTexturePath).c_str());
		// Copy texture data
		//data.FromSurface(rawImg);
		return true;
	}

	return false;
}
