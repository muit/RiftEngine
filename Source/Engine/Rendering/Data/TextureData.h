// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"
#include "Core/Containers/Array.h"


struct TextureData {
	using ColorBuffer = TArray<Color>;

private:
	v2_u32 size;
	ColorBuffer buffer;


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

	FORCEINLINE Color& operator[](const v2_u32 position) {
		// X + Y * Width
		return buffer[position.x() + position.y() * size.x()];
	}

	FORCEINLINE const Color& operator[](const v2_u32 position) const {
		// X + Y * Width
		return buffer[position.x() + position.y() * size.x()];
	}


	FORCEINLINE const v2_u32& Size() const { return size; }

	FORCEINLINE ColorBuffer& Buffer() { return buffer; }
	FORCEINLINE const ColorBuffer& Buffer() const { return buffer; }
};
