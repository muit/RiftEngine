// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Core/Strings/String.h"
#include "Core/Math/Vector.h"
#include "Resources/RenderMaterial.h"

struct TextureData;


struct ScreenTexture {

	GLuint textureId;

	GLuint square_vbo0;
	GLuint square_vbo1;

	RenderMaterial shaderProgram;


	ScreenTexture()
		: textureId{0}
		, square_vbo0{0}
		, square_vbo1{0}
		, shaderProgram{}
	{}
	ScreenTexture(u32 initialWidth, u32 initialHeight) : shaderProgram{ vertexShader, fragmentShader } {
		BuildFrame(initialWidth, initialHeight);
	}

	ScreenTexture(ScreenTexture&& other) { MoveFrom(MoveTemp(other)); }
	ScreenTexture& operator=(ScreenTexture&& other) {
		MoveFrom(MoveTemp(other));
		return *this;
	}


	void Draw(v2_u32 size, const TextureData& buffer);

	~ScreenTexture();

private:

	void BuildFrame(u32 width, u32 height);

	void MoveFrom(ScreenTexture&& other);

	// STATIC
	static const String vertexShader;
	static const String fragmentShader;
};
