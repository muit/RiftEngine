// © 2019 Miguel Fernández Arce - All rights reserved

#include "RenderTexture.h"
#include <SDL_surface.h>



bool RenderTexture::PostLoad()
{
	Super::PostLoad();

	return true;
}

void RenderTexture::Render()
{
	//glRasterPos2f(-1.f, +1.f);
	//glPixelZoom(+1.f, -1.f);
	//glDrawPixels(size.x(), size.y(), GL_RGBA, GL_UNSIGNED_BYTE, &buffer.First());
}
