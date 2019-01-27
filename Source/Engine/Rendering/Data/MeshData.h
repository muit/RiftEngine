// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"
#include "Core/Containers/Array.h"


using VertexBuffer = TArray<v3>;
using VertexBufferI32 = TArray<v3_i32>;
using NormalsBuffer = TArray<v3>;
using Triangle = v3_u32;
using TriangleBuffer = TArray<Triangle>;

struct MeshData {

	VertexBuffer vertices;
	NormalsBuffer normals;
	TriangleBuffer triangles;


public:

	MeshData() : vertices{
			{ -4, -4, +4 },      // 0   Testing vertices
			{ +4, -4, +4 },      // 1
			{ +4, +4, +4 },      // 2
			{ -4, +4, +4 },      // 3
			{ -4, -4, -4 },      // 4
			{ +4, -4, -4 },      // 5
			{ +4, +4, -4 },      // 6
			{ -4, +4, -4 },      // 7
			{ -5, -5,  0 },      // 8
			{ +5, -5,  0 },      // 9
			{ +5, +5,  0 },      // 10
			{ -5, +5,  0 },      // 11
			{  0, -5, +5 },      // 12
			{  0, +5, +5 },      // 13
			{  0, +5, -5 },      // 15
			{  0, -5, -5 }       // 14
		}, triangles {
			{  0,  1,  2 },         // cube front
			{  0,  2,  3 },
			{  4,  0,  3 },         // cube left
			{  4,  3,  7 },
			{  5,  4,  7 },         // cube back
			{  5,  7,  6 },
			{  1,  5,  6 },         // cube right
			{  1,  6,  2 },
			{  3,  2,  6 },         // cube top
			{  3,  6,  7 },
			{  0,  4,  5 },         // cube bottom
			{  0,  5,  1 },
			{  8,  9, 10 },         // middle front-face
			{  8, 10, 11 },
			{ 10,  9,  8 },         // middle back-face
			{ 11, 10,  8 },
			{ 12, 13, 14 },         // middle left-face
			{ 12, 14, 15 },
			{ 14, 13, 12 },         // middle right-face
			{ 15, 14, 12 }
		}
	{}

	void FromAssimpScene(const struct aiScene* scene);

	FORCEINLINE const VertexBuffer& GetVertices() const { return vertices; }
	FORCEINLINE const TriangleBuffer& GetTriangles() const { return triangles; }

private:

	void ProcessNode(struct aiNode *node, const struct aiScene* scene);
	void ProcessMesh(struct aiMesh *mesh, const struct aiScene* scene);
};
