// © 2019 Miguel Fernández Arce - All rights reserved

#include "Frame.h"

u16 Frame::idCounter = 0;


void Frame::ExecuteCommands(FrameRender& render)
{
	for (const auto& command : commands)
	{
		command->Execute(render, *this);
	}
	commands.Empty();
}


void FrameRender::DrawImage(const v2_u32& position, const TextureData& texture)
{
	u32 width = texture.Size().x();
	u32 height = texture.Size().y();
	u32 pitch = baseColor.Size().x();
	u32 offset = pitch * position.y() + position.x();
	u32 delta = pitch - width;

	const Color* texturePixel = texture.Buffer().Data();
	Color* bcPixel = baseColor.Buffer().Data() + offset;

	while (height-- > 0)
	{
		for (Color* end = bcPixel + width; bcPixel < end; bcPixel++, texturePixel++)
		{
			*bcPixel = *texturePixel;
		}

		bcPixel += delta;
	}
}
