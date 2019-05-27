// © 2019 Miguel Fernández Arce - All rights reserved

#include "RenderTexture.h"

void RenderTexture::Load(const TextureData& texture)
{
	glEnable(GL_TEXTURE_2D);

	const v2_u32 textureSize = texture.Size();

	glGenBuffers(1, &glId);
	glBindTexture(GL_TEXTURE_2D, glId);

	// Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	if (texture.filtering == ETextureFiltering::Linear)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Copy data to GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize.x, textureSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)texture.Buffer().Data());
	glGenerateMipmap(GL_TEXTURE_2D);
}
