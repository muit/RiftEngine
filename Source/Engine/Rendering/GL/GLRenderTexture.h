// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Core/Strings/String.h"
#include "Core/Math/Vector.h"
#include "GLProgram.h"

struct TextureData;


struct GLRenderTexture {

	GLuint textureId;

	GLuint square_vbo0;
	GLuint square_vbo1;

	GLProgram shaderProgram;


	GLRenderTexture()
		: textureId{0}
		, square_vbo0{0}
		, square_vbo1{0}
		, shaderProgram{}
	{}
	GLRenderTexture(u32 initialWidth, u32 initialHeight) : shaderProgram{ vertexShader, fragmentShader } {
		BuildFrame(initialWidth, initialHeight);
	}

	GLRenderTexture(GLRenderTexture&& other) { MoveFrom(MoveTemp(other)); }
	GLRenderTexture& operator=(GLRenderTexture&& other) {
		MoveFrom(MoveTemp(other));
		return *this;
	}


	void Draw(v2_u32 size, const TextureData& buffer);

	~GLRenderTexture();

private:

	void BuildFrame(u32 width, u32 height);

	void MoveFrom(GLRenderTexture&& other);

	// STATIC
	static const String vertexShader;
	static const String fragmentShader;
};
