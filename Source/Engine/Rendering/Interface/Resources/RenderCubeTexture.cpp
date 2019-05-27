// � 2019 Miguel Fern�ndez Arce - All rights reserved

#include "RenderCubeTexture.h"
#include "Rendering/Data/CubeTextureData.h"


const float RenderCubeTexture::cubeVertices[] {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
};

void RenderCubeTexture::Load(const CubeTextureData& cubeTexture)
{
	// Load textures

	glGenTextures(1, &glId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, glId);

	// Copy data to GPU
	for (GLuint i = 0; i < 6; ++i)
	{
		const auto& texture = cubeTexture.textures[i];
		const v2_u32 textureSize = texture.Size();

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, textureSize.x, textureSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)texture.Buffer().Data());
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	// Load Cube
	glGenVertexArrays(1, &glCubeVAO);
	glGenBuffers(1, &glCubeVBO);

	glBindVertexArray(glCubeVAO);

	// Bind
	glBindBuffer(GL_ARRAY_BUFFER, glCubeVBO);
	glBufferData(GL_ARRAY_BUFFER, 36, &cubeVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
}

void RenderCubeTexture::Free()
{
	glDeleteTextures(1, &glId);

	glDeleteVertexArrays(1, &glCubeVAO);
	glDeleteBuffers(1, &glCubeVBO);
}
