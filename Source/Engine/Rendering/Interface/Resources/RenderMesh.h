// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "../../Data/MeshData.h"


struct RenderMesh
{
	u32 glVerticesId  = GL_INVALID_INDEX;
	u32 glTrianglesId = GL_INVALID_INDEX;

	u32 glVertexArrayId = GL_INVALID_INDEX;

	u32 indicesCount = 0;
	u32 verticesCount = 0;


	RenderMesh(const MeshData& mesh) { Load(mesh); }
	~RenderMesh() { Free(); }

	void Bind() const;

	void Draw() const
	{
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
	}

	static void Unbind() { glBindVertexArray(0); }

private:

	void Load(const MeshData& mesh);

	void Free();
};

