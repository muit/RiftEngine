// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Rendering/Data/CubeTextureData.h"


struct RenderCubeTexture
{
	static const float cubeVertices[];

	u32 glId = GL_INVALID_INDEX;
	u32 glCubeVBO = GL_INVALID_INDEX;
	u32 glCubeVAO = GL_INVALID_INDEX;


	RenderCubeTexture() = default;
	RenderCubeTexture(const CubeTextureData& texture) { Load(texture); }
	~RenderCubeTexture() { Free(); }

	void DrawAsSkybox() const {
		glBindVertexArray(glCubeVAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, glId);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

private:

	void Load(const CubeTextureData& cubeTexture);

	void Free();
};

