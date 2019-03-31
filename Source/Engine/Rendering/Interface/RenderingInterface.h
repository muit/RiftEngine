// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreEngine.h"

#include "../Data/TextureData.h"
#include "../Data/MeshData.h"


namespace GLTextures
{
	static u32 Bind(const TextureData& texture)
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

	static void Free(u32 glTextureId)
	{
		glDeleteTextures(1, &glTextureId);
	}
};

namespace GLMeshes
{
};

class RenderingInterface
{
public:

	u32 BindTexture(const TextureData& texture);

	void FreeTexture(u32 glTextureId);

	u32 BindMesh(const MeshData& texture);
	void FreeMesh(u32 glTextureId);


};
