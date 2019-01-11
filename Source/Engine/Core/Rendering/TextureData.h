// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"
#include "Core/Containers/Array.h"


struct TextureData {
	using PixelBuffer = TArray<Color>;

private:
	v2_u32 size;
	PixelBuffer buffer;


public:

	TextureData(v2_u32 size = v2_u32::Zero()) : size{ size }, buffer{size.x() * size.y()} {}

	void Fill(Color color);

	void FromSurface(struct SDL_Surface* surface);

	// Receives a surface and updates all its pixels
	void ToSurface(struct SDL_Surface* surface);

	const v2_u32& Size() const { return size; }

	PixelBuffer& Buffer() { return buffer; }
	const PixelBuffer& Buffer() const { return buffer; }
};
