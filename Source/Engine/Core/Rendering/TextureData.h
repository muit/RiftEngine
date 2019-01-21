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

	TextureData(v2_u32 inSize = v2_u32::Zero()) : buffer{} {
		Resize(inSize);
	}

	void Resize(v2_u32 inSize) {
		size = inSize;
		buffer.Resize(size.x() * size.y());
	}

	void Resize(v2_u32 inSize, const Color& clearColor) {
		size = inSize;
		buffer.Empty(false);
		buffer.Resize(size.x() * size.y(), clearColor);
	}

	void Fill(const Color& color) {
		Resize(size, color);
	}

	void FromSurface(struct SDL_Surface* surface);

	// Receives a surface and updates all its pixels
	void ToSurface(struct SDL_Surface* surface);

	const v2_u32& Size() const { return size; }

	PixelBuffer& Buffer() { return buffer; }
	const PixelBuffer& Buffer() const { return buffer; }
};
