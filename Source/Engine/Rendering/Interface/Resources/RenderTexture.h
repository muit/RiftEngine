// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "../../Data/TextureData.h"


struct RenderTexture
{
	u32 glId = GL_INVALID_INDEX;


	RenderTexture() = default;
	RenderTexture(const TextureData& texture) { Load(texture); }
	~RenderTexture() { Free(); }

	void Bind()
	{
		glBindTexture(GL_TEXTURE_2D, glId);
	}

private:

	void Load(const TextureData& texture);

	void Free()
	{
		glDeleteTextures(1, &glId);
	}
};

