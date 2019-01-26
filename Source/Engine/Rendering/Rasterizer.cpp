// © 2019 Miguel Fernández Arce - All rights reserved
// From Angel Rodriguez's original code under public domain.
// angel.rodriguez@esne.edu

#include "Rasterizer.h"


void Rasterizer::FillConvexPolygon(const VertexBufferI32& vertices, const u32* indicesBegin, const u32* indicesEnd, const Color& color)
{
	// Se cachean algunos valores de i32erés:

	u32 pitch = target.Size().x();
	i32* offsetCache0Index = this->offsetCache0;
	i32* offsetCache1Index = this->offsetCache1;
	const u32* indices_back = indicesEnd - 1;

	// Se busca el vértice de inicio (el que tiene menor Y) y el de terminación (el que tiene mayor Y):
	const u32* startIndex = indicesBegin;
	i32 startY = vertices[*startIndex][1];

	const u32* endIndex = indicesBegin;
	i32 endY = startY;

	for (const u32* it = startIndex; ++it < indicesEnd; )
	{
		i32 currentY = vertices[*it][1];

		if (currentY < startY)
		{
			startY = currentY;
			startIndex = it;
		}
		else
			if (currentY > endY)
			{
				endY = currentY;
				endIndex = it;
			}
	}

	// Se cachean las coordenadas X de los lados que van desde el vértice con Y menor al
	// vértice con Y mayor en sentido antihorario:

	const u32* cIndex = startIndex;
	const u32* nextIndex = (startIndex > indicesBegin) ? startIndex - 1 : indices_back;

	i32 y0 = vertices[*cIndex][1];
	i32 y1 = vertices[*nextIndex][1];
	i32 o0 = vertices[*cIndex][0] + y0 * pitch;
	i32 o1 = vertices[*nextIndex][0] + y1 * pitch;

	while (true)
	{
		Interpolate< i64, 32 >(offsetCache0Index, o0, o1, y0, y1);

		cIndex = (cIndex == indicesBegin) ? indices_back : cIndex - 1;

		if (cIndex == endIndex)
			break;

		nextIndex = (nextIndex == indicesBegin) ? indices_back : nextIndex - 1;

		y0 = y1;
		y1 = vertices[*nextIndex][1];
		o0 = o1;
		o1 = vertices[*nextIndex][0] + y1 * pitch;
	}

	i32 end_offset = o1;

	// Se cachean las coordenadas X de los lados que van desde el vértice con Y menor al
	// vértice con Y mayor en sentido horario:

	cIndex = startIndex;
	nextIndex = startIndex < indices_back ? startIndex + 1 : indicesBegin;

	y0 = vertices[*cIndex][1];
	y1 = vertices[*nextIndex][1];
	o0 = vertices[*cIndex][0] + y0 * pitch;
	o1 = vertices[*nextIndex][0] + y1 * pitch;

	while (true)
	{
		Interpolate<i64, 32>(offsetCache1Index, o0, o1, y0, y1);

		if (cIndex == indices_back) cIndex = indicesBegin; else cIndex++;
		if (cIndex == endIndex) break;
		if (nextIndex == indices_back) nextIndex = indicesBegin; else nextIndex++;

		y0 = y1;
		y1 = vertices[*nextIndex][1];
		o0 = o1;
		o1 = vertices[*nextIndex][0] + y1 * pitch;
	}

	if (o1 > end_offset) end_offset = o1;

	// Se rellenan las scanlines desde la que tiene menor Y hasta la que tiene mayor Y:

	offsetCache0Index += startY;
	offsetCache1Index += startY;

	for (i32 y = startY; y < endY; y++)
	{
		o0 = *offsetCache0Index++;
		o1 = *offsetCache1Index++;

		if (o0 < o1)
		{
			while (o0 < o1)
				target.Buffer()[o0++] = color;

			if (o0 > end_offset) break;
		}
		else
		{
			while (o1 < o0)
				target.Buffer()[o1++] = color;

			if (o1 > end_offset) break;
		}
	}
}

void Rasterizer::FillConvexPolygonZBuffer(const VertexBufferI32& vertices, const u32* const indicesBegin, const u32* const indicesEnd, const Color& color)
{
	// Se cachean algunos valores de interés:

	i32 pitch = target.Size().x();
	i32* offsetCache0Index = this->offsetCache0;
	i32* offsetCache1Index = this->offsetCache1;
	i32* zCache0Index = this->zCache0;
	i32* zCache1Index = this->zCache1;
	const u32* indicesBack = indicesEnd - 1;

	// Se busca el vértice de inicio (el que tiene menor Y) y el de terminación (el que tiene mayor Y):

	const u32* startIndex = indicesBegin;
	i32 startY = vertices[*startIndex][1];
	const u32* endIndex = indicesBegin;
	i32 endY = startY;

	for (const u32* it = startIndex; ++it < indicesEnd; )
	{
		i32 currentY = vertices[*it][1];

		if (currentY < startY)
		{
			startY = currentY;
			startIndex = it;
		}
		else if (currentY > endY)
		{
			endY = currentY;
			endIndex = it;
		}
	}

	// Se cachean las coordenadas X de los lados que van desde el vértice con Y menor al
	// vértice con Y mayor en sentido antihorario:

	const u32* cIndex = startIndex;
	const u32* nextIndex = startIndex > indicesBegin ? startIndex - 1 : indicesBack;

	i32 y0 = vertices[*cIndex][1];
	i32 y1 = vertices[*nextIndex][1];
	i32 z0 = vertices[*cIndex][2];
	i32 z1 = vertices[*nextIndex][2];
	i32 o0 = vertices[*cIndex][0] + y0 * pitch;
	i32 o1 = vertices[*nextIndex][0] + y1 * pitch;

	while (true)
	{
		Interpolate< i64, 32 >(offsetCache0Index, o0, o1, y0, y1);
		Interpolate< i32, 0 >(zCache0Index, z0, z1, y0, y1);

		if (cIndex == indicesBegin) cIndex = indicesBack; else cIndex--;
		if (cIndex == endIndex) break;
		if (nextIndex == indicesBegin) nextIndex = indicesBack; else    nextIndex--;

		y0 = y1;
		y1 = vertices[*nextIndex][1];
		z0 = z1;
		z1 = vertices[*nextIndex][2];
		o0 = o1;
		o1 = vertices[*nextIndex][0] + y1 * pitch;
	}

	i32 end_offset = o1;

	// Se cachean las coordenadas X de los lados que van desde el vértice con Y menor al
	// vértice con Y mayor en sentido horario:

	cIndex = startIndex;
	nextIndex = startIndex < indicesBack ? startIndex + 1 : indicesBegin;

	y0 = vertices[*cIndex][1];
	y1 = vertices[*nextIndex][1];
	z0 = vertices[*cIndex][2];
	z1 = vertices[*nextIndex][2];
	o0 = vertices[*cIndex][0] + y0 * pitch;
	o1 = vertices[*nextIndex][0] + y1 * pitch;

	while (true)
	{
		Interpolate< i64, 32 >(offsetCache1, o0, o1, y0, y1);
		Interpolate< i32, 0 >(zCache1Index, z0, z1, y0, y1);

		if (cIndex == indicesBack)
			cIndex = indicesBegin;
		else
			++cIndex;

		if (cIndex == endIndex)
			break;

		if (nextIndex == indicesBack)
			nextIndex = indicesBegin;
		else
			++nextIndex;

		y0 = y1;
		y1 = vertices[*nextIndex][1];
		z0 = z1;
		z1 = vertices[*nextIndex][2];
		o0 = o1;
		o1 = vertices[*nextIndex][0] + y1 * pitch;
	}

	if (o1 > end_offset)
		end_offset = o1;

	// Se rellenan las scanlines desde la que tiene menor Y hasta la que tiene mayor Y:

	offsetCache0Index += startY;
	offsetCache1Index += startY;
	zCache0Index += startY;
	zCache1Index += startY;

	for (i32 y = startY; y < endY; y++)
	{
		o0 = *offsetCache0Index++;
		o1 = *offsetCache1Index++;
		z0 = *zCache0Index++;
		z1 = *zCache1Index++;

		if (o0 < o1)
		{
			i32 z_step = (z1 - z0) / (o1 - o0);

			while (o0 < o1)
			{
				if (z0 < zBuffer[o0]) // #Fix: Array out of bounds here
				{
					target.Buffer()[o0] = color;
					zBuffer[o0] = z0;
				}

				z0 += z_step;
				o0++;
			}

			if (o0 > end_offset) break;
		}
		else if (o1 < o0)
		{
			i32 z_step = (z0 - z1) / (o0 - o1);

			while (o1 < o0)
			{
				if (z1 < zBuffer[o1])
				{
					target.Buffer()[o1] = color;
					zBuffer[o1] = z1;
				}

				z1 += z_step;
				o1++;
			}

			if (o1 > end_offset) break;
		}
	}
}

void Rasterizer::FillTriangle(const VertexBufferI32& vertices, const v3_u32& triangle)
{
	float triangleDepth;
	box2_i32 bounds = GetTriangleBounds(vertices, triangle, triangleDepth);

	if (triangleDepth < 0.f)
		return;

	// Frustum culling by limiting bounds
	bounds.Cut(viewportBounds);

	for (i32 rasterX = bounds.min.x(); rasterX <= bounds.max.x(); ++rasterX)
	{
		for (i32 rasterY = bounds.min.y(); rasterY <= bounds.max.y(); ++rasterY)
		{
			const v2_i32 pixel = { rasterX, rasterY };
			if (IsPixelInsideTriangle(vertices, triangle, pixel))
			{
				//target[pixel] = Color::Cyan;
			}
		}
	}
}

void Rasterizer::FillVertexBuffer(const VertexBufferI32& vertices, const TriangleBuffer& triangles, const Color& color)
{
	for (const auto& triangle : triangles)
	{
		//FillTriangle(vertices, triangle);
		FillConvexPolygonZBuffer(vertices, triangle.data(), triangle.data() + 3, color);
	}
}

box2_i32 Rasterizer::GetTriangleBounds(const VertexBufferI32& vertices, const v3_u32& triangle, float& depth) const
{
	static const box2_i32 minimumBounds{
		{ std::numeric_limits<i32>::max(), std::numeric_limits<i32>::max() },
		{ std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min() }
	};
	box2_i32 bounds = minimumBounds;

	depth = 0.f;
	for (u32 i = 0; i < 3; ++i)
	{
		const v3_i32& vertex = vertices[triangle[i]];
		bounds.ExtendPoint(
			vertices[triangle[i]].head<2>()
		);

		depth += vertex.z();
	}
	// Average depth
	depth *= 0.333f;

	return bounds;
}

bool Rasterizer::IsPixelInsideTriangle(const VertexBufferI32& vertices, const v3_u32& triangle, const v2_i32& pixel) const
{
	float u, v, w;
	{// Barycentric Coordinates
		const v2_i32 vertex0{ vertices[triangle[0]].head<2>() };

		const v2_i32 p0 = vertices[triangle[1]].head<2>() - vertex0;
		const v2_i32 p1 = vertices[triangle[2]].head<2>() - vertex0;
		const v2_i32 p2 = pixel                           - vertex0;

		const i32 d00 = p0.dot(p0);
		const i32 d01 = p0.dot(p1);
		const i32 d11 = p1.dot(p1);
		const i32 d20 = p2.dot(p0);
		const i32 d21 = p2.dot(p1);
		i32 denom = d00 * d11 - d01 * d01;
		v = (d11 * d20 - d01 * d21) / denom;
		w = (d00 * d21 - d01 * d20) / denom;
		u = 1.0f - v - w;
	}
	return u >= 0 && v >= 0 && u + v <= 1.0;
}
