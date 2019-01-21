// © 2019 Miguel Fernández Arce - All rights reserved

#include "Texture.h"
#include <SDL_surface.h>
#include <SDL_image.h>

#include "Core/Assets/AssetInfo.h"
#include "Core/Files/FileSystem.h"


bool Texture::PostLoad()
{
	Super::PostLoad();

	Path rawTexturePath = FileSystem::FindRawFile(FileSystem::FromString(Info().GetSPath()));
	if(!rawTexturePath.empty())
	{
		SDL_Surface* rawImg = IMG_Load(FileSystem::ToString(rawTexturePath).c_str());
		// Copy texture data
		data.FromSurface(rawImg);
		SDL_FreeSurface(rawImg);
		return true;
	}

	return false;
}
