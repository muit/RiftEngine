// © 2019 Miguel Fernández Arce - All rights reserved
// From Angel Rodriguez's original code under public domain.
// angel.rodriguez@esne.edu

#pragma once

#include "Data/TextureData.h"
#include "Data/MeshData.h"


class Rasterizer
{
	using VertexBuffer = MeshData::VertexBuffer;
	using TriangleBuffer = MeshData::TriangleBuffer;


	TextureData* target;

	int offsetCache0[1082];
	int offsetCache1[1082];

	int zCache0[1082];
	int zCache1[1082];

	TArray<i32> zBuffer;


public:

	Rasterizer()
		: target{nullptr}
		, zBuffer{}
	{}

	void RebindTexture(TextureData& inTarget)
	{
		target = &inTarget;
		// Update zBuffer
		zBuffer.Resize(target->Buffer().Size(), eastl::numeric_limits<i32>::max());
	}

	const TextureData& GetTargetTexture() const
	{
		return *target;
	}

public:

	void FillConvexPolygon(const VertexBuffer& vertices, const u32* indicesBegin, const u32* indicesEnd, const Color& color);

	void FillConvexPolygonZBuffer(const VertexBuffer& vertices, const u32* indicesBegin, const u32* indicesEnd, const Color& color);

	void FillVertexBuffer(const VertexBuffer& vertices, const TriangleBuffer& triangles, const Color& color);

private:

	template< typename VALUE_TYPE, size_t SHIFT >
	void Interpolate(i32* cache, i32 v0, i32 v1, i32 yMin, i32 yMax);
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
