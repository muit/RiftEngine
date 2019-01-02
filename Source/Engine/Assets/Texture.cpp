// Copyright 2017/2018 - Miguel Fernández Arce

#include "Texture.h"
#include <SDL_surface.h>
#include "Core/Assets/AssetInfo.h"


bool Texture::PostLoad()
{
	SDL_Surface* img = SDL_LoadBMP(Info().GetSPath().c_str());

	return true;
}
