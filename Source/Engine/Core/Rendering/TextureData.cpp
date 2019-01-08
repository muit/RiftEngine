// © 2019 Miguel Fernández Arce - All rights reserved

#include "TextureData.h"
#include "SDL_surface.h"


void TextureData::FromSurface(SDL_Surface* surface)
{
	if (surface)
	{
		bool reformated = false;
		if (surface->format->format != SDL_PIXELFORMAT_RGBA32)
		{
			surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
			reformated = true;
		}

		size = { surface->w, surface->h };

		const uint32 pixelCount = size.x() * size.y();
		uint32* const pixels = (uint32*)surface->pixels;

		// Copy pixels
		buffer.Reserve(pixelCount);
		for (uint32 i = 0; i < pixelCount; ++i)
		{
			const uint32 pixel = pixels[i];
			buffer.Add(Color{ pixel });
		}

		if (reformated) {
			SDL_FreeSurface(surface);
		}
	}
}
