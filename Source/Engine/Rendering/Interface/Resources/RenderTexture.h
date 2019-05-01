// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "../../Data/TextureData.h"


struct RenderTexture
{
	u32 glId = GL_INVALID_INDEX;


	RenderTexture(const TextureData& texture) { Load(texture); }
	~RenderTexture() { Free(); }

	void Bind()
	{
		glBindTexture(GL_TEXTURE_2D, glId);
	}

private:

	void Load(const TextureData& texture)
	{
		glEnable(GL_TEXTURE_2D);

		const v2_u32 textureSize = texture.Size();

		glGenBuffers(1, &glId);
		glBindTexture(GL_TEXTURE_2D, glId);

		// Copy data to GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize.x, textureSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)texture.Buffer().Data());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void Free()
	{
		glDeleteTextures(1, &glId);
	}
};

