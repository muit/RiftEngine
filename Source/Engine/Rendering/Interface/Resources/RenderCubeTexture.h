// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Rendering/Data/CubeTextureData.h"


struct RenderCubeTexture
{
	u32 glId = GL_INVALID_INDEX;


	RenderCubeTexture() = default;
	RenderCubeTexture(const CubeTextureData& texture) { Load(texture); }
	~RenderCubeTexture() { Free(); }

	void Bind() const {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, glId);
	}

private:

	void Load(const CubeTextureData& cubeTexture);

	void Free();
};

