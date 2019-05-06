// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "../../Data/MeshData.h"
#include "Core/Assets/AssetInfo.h"


struct RenderMesh
{
	u32 glVerticesId = GL_INVALID_INDEX;
	u32 glTrianglesId = GL_INVALID_INDEX;

	u32 glVertexArrayId = GL_INVALID_INDEX;

	u32 indicesCount = 0;
	u32 verticesCount = 0;


	RenderMesh(Name id, const MeshData& mesh) { Load(id, mesh); }
	~RenderMesh() { Free(); }
	RenderMesh(RenderMesh&& other)            { MoveFrom(MoveTemp(other)); }
	RenderMesh& operator=(RenderMesh&& other) { MoveFrom(MoveTemp(other)); return *this; }

	void Bind() const
	{
		glBindVertexArray(glVertexArrayId);
	}

	void Draw() const
	{
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
	}

	static void Unbind() { glBindVertexArray(0); }

private:

	void Load(Name id, const MeshData& mesh);

	void Free();

	void MoveFrom(RenderMesh&& other)
	{
		glVertexArrayId = other.glVertexArrayId;
		glVerticesId    = other.glVerticesId;
		glTrianglesId   = other.glTrianglesId;
		indicesCount    = other.indicesCount;
		verticesCount   = other.verticesCount;
		other.glVertexArrayId = GL_INVALID_INDEX;
		other.glVerticesId    = GL_INVALID_INDEX;
		other.glTrianglesId   = GL_INVALID_INDEX;
		other.indicesCount = 0;
		other.glVerticesId = 0;
	}
};
