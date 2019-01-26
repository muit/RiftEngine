// © 2019 Miguel Fernández Arce - All rights reserved
// From Angel Rodriguez's original code under public domain.
// angel.rodriguez@esne.edu

#pragma once

#include "Data/TextureData.h"
#include "Data/MeshData.h"


class Rasterizer
{
	box2_i32 viewportBounds;
	TextureData& target;

	TArray<i32> zBuffer;

	i32 offsetCache0[1082];
	i32 offsetCache1[1082];

	i32 zCache0[1082];
	i32 zCache1[1082];



public:

	Rasterizer(TextureData& target)
		: target{target}
		, zBuffer{}
		, offsetCache0{}
		, offsetCache1{}
		, zCache0{}
		, zCache1{}
	{}

	void Clear()
	{
		// Update zBuffer
		zBuffer.Empty(false);
		zBuffer.Resize(target.Buffer().Size(), eastl::numeric_limits<i32>::max());

		viewportBounds = {};
		viewportBounds.ExtendPoint(target.Size().cast<i32>());

	}

	void FillConvexPolygon(const VertexBufferI32& vertices, const u32* indicesBegin, const u32* indicesEnd, const Color& color);

	void FillConvexPolygonZBuffer(const VertexBufferI32& vertices, const u32* indicesBegin, const u32* indicesEnd, const Color& color);

	void FillTriangle(const VertexBufferI32& vertices, const v3_u32& triangle);

	void FillVertexBuffer(const VertexBufferI32& vertices, const TriangleBuffer& triangles, const Color& color);


	const TextureData& GetTargetTexture() const { return target; }

private:

	template< typename VALUE_TYPE, size_t SHIFT >
	void Interpolate(i32* cache, i32 v0, i32 v1, i32 yMin, i32 yMax);

	// Triangle operations
	box2_i32 GetTriangleBounds(const VertexBufferI32& vertices, const v3_u32& triangle, float& depth) const;
	bool IsPixelInsideTriangle(const VertexBufferI32& vertices, const v3_u32& triangle, const v2_i32& pixel) const;
};


template< typename Type, size_t SHIFT >
void Rasterizer::Interpolate(i32* cache, i32 v0, i32 v1, i32 yMin, i32 yMax)
{
	if (yMax > yMin)
	{
		Type value = (Type(v0) << SHIFT);
		Type step = (Type(v1 - v0) << SHIFT) / (yMax - yMin);

		for (i32* it = cache + yMin, *end = cache + yMax; it <= end; )
		{
			*it++ = i32(value >> SHIFT);
			value += step;
			*it++ = i32(value >> SHIFT);
			value += step;
		}
	}
}
