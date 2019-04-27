// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"
#include "Core/Containers/Array.h"


struct Vertex
{
	v3 position;
	v3 normal;
	v2 uv;
};

using VertexBuffer = TArray<Vertex>;
using Triangle = v3_u32;
using TriangleBuffer = TArray<Triangle>;

using VertexPositionBufferI32 = TArray<v3_i32>;


struct MeshData
{
	VertexBuffer vertices;
	TriangleBuffer triangles;


public:

	MeshData() = default;

	void FromAssimpScene(const struct aiScene* scene);

	FORCEINLINE const VertexBuffer& GetVertices() const { return vertices; }
	FORCEINLINE const TriangleBuffer& GetTriangles() const { return triangles; }

private:

	void ProcessNode(struct aiNode *node, const struct aiScene* scene);
	void ProcessMesh(struct aiMesh *mesh, const struct aiScene* scene);
};
