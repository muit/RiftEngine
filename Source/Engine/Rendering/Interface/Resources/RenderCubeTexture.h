// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "../../Data/CubeTextureData.h"


struct RenderCubeTexture
{
	u32 glId = GL_INVALID_INDEX;


	RenderCubeTexture() = default;
	RenderCubeTexture(const CubeTextureData& texture) { Load(texture); }
	~RenderCubeTexture() { Free(); }

	void Bind() {}

private:

	void Load(const CubeTextureData& texture) {}

	void Free() {}
};

