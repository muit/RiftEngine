// © 2019 Miguel Fernández Arce - All rights reserved

#include "TextureData.h"
#include "SDL_surface.h"


bool TextureData::FromSurface(SDL_Surface* surface)
{
	if (surface)
	{
		bool bReformated = false;
		if (surface->format->format != SDL_PIXELFORMAT_RGBA32)
		{
			surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
			bReformated = true;
		}

		size = { surface->w, surface->h };

		const u32 pixelCount = size.x * size.y;
		u32* const pixels = (u32*)surface->pixels;

		// Copy pixels
		buffer.Reserve(pixelCount);
		for (u32 i = 0; i < pixelCount; ++i)
		{
			buffer.Add(Color{ pixels[i] });
		}

		if (bReformated)
		{
			SDL_FreeSurface(surface);
		}
		return true;
	}
	return false;
}
