// © 2019 Miguel Fernández Arce - All rights reserved
// From Angel Rodriguez's original code under public domain.
// angel.rodriguez@esne.edu

#include "Rasterizer.h"


void Rasterizer::FillConvexPolygon(const VertexBuffer& vertices, const u32* indicesBegin, const u32* indicesEnd, const Color& color)
{
	// Se cachean algunos valores de interés:

	u32 pitch = target.Size().x();
	int* offsetCache0Index = this->offsetCache0;
	int* offsetCache1Index = this->offsetCache1;
	const u32* indices_back = indicesEnd - 1;

	// Se busca el vértice de inicio (el que tiene menor Y) y el de terminación (el que tiene mayor Y):
	const u32* startIndex = indicesBegin;
	int startY = vertices[*startIndex][1];

	const u32* endIndex = indicesBegin;
	int endY = startY;

	for (const u32* it = startIndex; ++it < indicesEnd; )
	{
		int currentY = vertices[*it][1];

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

	int y0 = vertices[*cIndex][1];
	int y1 = vertices[*nextIndex][1];
	int o0 = vertices[*cIndex][0] + y0 * pitch;
	int o1 = vertices[*nextIndex][0] + y1 * pitch;

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

	int end_offset = o1;

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

	for (int y = startY; y < endY; y++)
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

void Rasterizer::FillConvexPolygonZBuffer(const VertexBuffer& vertices, const u32* const indicesBegin, const u32* const indicesEnd, const Color& color)
{
	// Se cachean algunos valores de interés:

	i32 pitch = target.Size().x();
	int* offsetCache0Index = this->offsetCache0;
	int* offsetCache1Index = this->offsetCache1;
	i32* zCache0Index = this->zCache0;
	i32* zCache1Index = this->zCache1;
	const u32* indicesBack = indicesEnd - 1;

	// Se busca el vértice de inicio (el que tiene menor Y) y el de terminación (el que tiene mayor Y):

	const u32* startIndex = indicesBegin;
	int   startY = vertices[*startIndex][1];
	const u32* endIndex = indicesBegin;
	int   endY = startY;

	for (const u32* it = startIndex; ++it < indicesEnd; )
	{
		int currentY = vertices[*it][1];

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

	int end_offset = o1;

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

	for (int y = startY; y < endY; y++)
	{
		o0 = *offsetCache0Index++;
		o1 = *offsetCache1Index++;
		z0 = *zCache0Index++;
		z1 = *zCache1Index++;

		if (o0 < o1)
		{
			int z_step = (z1 - z0) / (o1 - o0);

			while (o0 < o1)
			{
				if (z0 < z_buffer[o0])
				{
					target.Buffer()[o0] = color;
					z_buffer[o0] = z0;
				}

				z0 += z_step;
				o0++;
			}

			if (o0 > end_offset) break;
		}
		else
			if (o1 < o0)
			{
				int z_step = (z0 - z1) / (o0 - o1);

				while (o1 < o0)
				{
					if (z1 < z_buffer[o1])
					{
						target.Buffer()[o1] = color;
						z_buffer[o1] = z1;
					}

					z1 += z_step;
					o1++;
				}

				if (o1 > end_offset) break;
			}
	}
}