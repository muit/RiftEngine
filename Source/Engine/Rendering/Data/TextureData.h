// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"
#include "Core/Containers/Array.h"


using ColorBuffer = TArray<Color>;
using LColorBuffer = TArray<LinearColor>;

struct TextureData {
private:
	v2_u32 size;
	ColorBuffer buffer;


public:

	TextureData(v2_u32 inSize = v2_u32::Zero()) : size{inSize}, buffer { inSize.x * inSize.y } {}

	void Resize(v2_u32 inSize) {
		size = inSize;
		buffer.Resize(size.x * size.y);
	}

	void Fill(const Color& color) {
		Clear(size, color);
	}

	void Clear(v2_u32 inSize, Color color = Color::Black) {
		size = inSize;
		buffer.Assign(size.x * size.y, color);
	}

	void FromSurface(struct SDL_Surface* surface);

	FORCEINLINE Color& operator[](const v2_u32& position) {
		// X + Y * Width
		return buffer[position.x + position.y * size.x];
	}

	FORCEINLINE const Color& operator[](const v2_u32& position) const {
		// X + Y * Width
		return buffer[position.x + position.y * size.x];
	}

	FORCEINLINE Color& operator[](const v2_i32& position) {
		assert(position.x >= 0 && position.y >= 0);
		// X + Y * Width
		return buffer[position.x + position.y * size.x];
	}

	FORCEINLINE const v2_u32& Size() const { return size; }

	FORCEINLINE ColorBuffer& Buffer() { return buffer; }
	FORCEINLINE const ColorBuffer& Buffer() const { return buffer; }
};
