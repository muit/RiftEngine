// Copyright 2017/2018 - Miguel Fernández Arce

#include "Texture.h"
#include <SDL_surface.h>
#include "Core/Assets/AssetInfo.h"
#include "SDL_opengl.h"


bool Texture::PostLoad()
{
	//SDL_Surface* img = SDL_LoadBMP(Info().GetSPath().c_str());

	return true;
}

void Texture::Render()
{
	//glRasterPos2f(-1.f, +1.f);
	//glPixelZoom(+1.f, -1.f);
	//glDrawPixels(size.x(), size.y(), GL_RGBA, GL_UNSIGNED_BYTE, &buffer.First());
}
