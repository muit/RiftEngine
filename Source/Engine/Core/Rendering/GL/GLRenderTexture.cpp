// © 2019 Miguel Fernández Arce - All rights reserved

#include "GLRenderTexture.h"
#include "../TextureData.h"


const String GLRenderTexture::vertexShader {
	"#version 330\n"
	""
	"layout (location = 0) in vec3 vertex_coordinates;"
	"layout (location = 1) in vec2 vertex_texture_uv;"
	""
	"out vec2 texture_uv;"
	""
	"void main()"
	"{"
	"   gl_Position = vec4(vertex_coordinates, 1.0);"
	"   texture_uv  = vertex_texture_uv;"
	"}"
};

const String GLRenderTexture::fragmentShader {
	"#version 330\n"
	""
	"uniform sampler2D sampler2d;"
	""
	"in  vec2 texture_uv;"
	"out vec4 fragment_color;"
	""
	"void main()"
	"{"
	"    fragment_color = vec4(texture(sampler2d, texture_uv.st).rgba);"
	"}"
};


void GLRenderTexture::BuildFrame(u32 width, u32 height)
{
	// Create texture
	{
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		// Texture format is RGB
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	static const GLfloat squareVertices[] =
	{
		+1.0f, -1.0f, 0.0f,
		+1.0f, +1.0f, 0.0f,
		-1.0f, +1.0f, 0.0f,

		-1.0f, +1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		+1.0f, -1.0f, 0.0f
	};

	static const GLfloat squareTextureUV[] =
	{
		+1.0f,  0.0f,
		+1.0f, +1.0f,
		 0.0f, +1.0f,

		 0.0f, +1.0f,
		 0.0f,  0.0f,
		+1.0f,  0.0f
	};

	glGenBuffers(1, &square_vbo0);
	glBindBuffer(GL_ARRAY_BUFFER, square_vbo0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &square_vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, square_vbo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareTextureUV), squareTextureUV, GL_STATIC_DRAW);
}

void GLRenderTexture::Draw(v2_u32 size, const TextureData& buffer)
{
	glViewport(0, 0, size.x(), size.y());
	shaderProgram.Use();


	glBindTexture(GL_TEXTURE_2D, textureId);

	// Update texture data
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.x(), size.y(), GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)buffer.Buffer().Data());

	// Render texture
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, square_vbo0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, square_vbo1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

GLRenderTexture::~GLRenderTexture()
{
	glDeleteTextures(1, &textureId);
	glDeleteBuffers(1, &square_vbo0);
	glDeleteBuffers(1, &square_vbo1);
}
