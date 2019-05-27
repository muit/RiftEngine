// � 2019 Miguel Fern�ndez Arce - All rights reserved

#include "RenderCubeTexture.h"
#include "Rendering/Data/CubeTextureData.h"


void RenderCubeTexture::Load(const CubeTextureData& cubeTexture)
{
	glEnable(GL_TEXTURE_CUBE_MAP);

	// Load textures
	glGenTextures(1, &glId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, glId);

	// Copy data to GPU
	for (GLuint i = 0; i < 6; ++i)
	{
		const auto& texture = cubeTexture.textures[i];
		const v2_u32 textureSize = texture.Size();

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, textureSize.x, textureSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)texture.Buffer().Data());
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void RenderCubeTexture::Free()
{
	glDeleteTextures(1, &glId);
}
