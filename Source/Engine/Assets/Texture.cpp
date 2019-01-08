// © 2019 Miguel Fernández Arce - All rights reserved

#include "Texture.h"
#include <SDL_surface.h>

#include "Core/Assets/AssetInfo.h"
#include "Core/Files/FileSystem.h"


bool Texture::PostLoad()
{
	Super::PostLoad();

	Path rawTexturePath = FileSystem::FindRawFile(FileSystem::FromString(Info().GetSPath()));
	if(!rawTexturePath.empty())
	{
		SDL_Surface* rawImg = SDL_LoadBMP(FileSystem::ToString(rawTexturePath).c_str());
		// Copy texture data
		data.FromSurface(rawImg);
		SDL_FreeSurface(rawImg);
	}

	return true;
}

void Texture::BeforeDestroy()
{
	Super::BeforeDestroy();
}

void Texture::Render()
{
	//glRasterPos2f(-1.f, +1.f);
	//glPixelZoom(+1.f, -1.f);
	//glDrawPixels(size.x(), size.y(), GL_RGBA, GL_UNSIGNED_BYTE, &buffer.First());
}
