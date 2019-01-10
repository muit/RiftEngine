// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"
#include "Core/Containers/Array.h"



struct TextureData {
	using PixelBuffer = TArray<Color>;

	v2_u32 size;
	PixelBuffer buffer;


	TextureData(v2_u32 size = v2_u32::Zero()) : size{ size } {}

	void FromSurface(struct SDL_Surface* surface);
};
