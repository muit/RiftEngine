// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "../../Data/TextureData.h"
#include "../OpenGL.h"


struct RenderTexture
{
private:
	u32 glId = GL_INVALID_INDEX;

public:

	RenderTexture() = default;
	RenderTexture(const TextureData& texture) { Load(texture); }
	~RenderTexture() { Free(); }

	NOINLINE void Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, glId);
		glCheckError();
	}

private:

	void Load(const TextureData& texture);

	void Free()
	{
		glDeleteTextures(1, &glId);
	}
};

