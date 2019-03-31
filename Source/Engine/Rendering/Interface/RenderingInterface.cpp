// Copyright 2015-2019 Piperift - All rights reserved

#include "RenderingInterface.h"


u32 RenderingInterface::Bind(const TextureData& texture)
{
	glEnable(GL_TEXTURE_2D);

	const v2_u32 textureSize = texture.Size();
	u32 glTextureId;
	glGenBuffers(1, &glTextureId);
	glBindTexture(GL_TEXTURE_2D, glTextureId);

	// Copy data to GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize.x(), textureSize.y(), 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)texture.Buffer().Data());
	glGenerateMipmap(GL_TEXTURE_2D);

	return glTextureId;
}

void RenderingInterface::Free(u32 glTextureId)
{
	glDeleteTextures(1, &glTextureId);
}
