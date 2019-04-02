// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "../../Data/MeshData.h"


struct RenderMesh
{
	u32 glVerticesId  = GL_INVALID_INDEX;
	u32 glNormalsId   = GL_INVALID_INDEX;
	u32 glTrianglesId = GL_INVALID_INDEX;

	u32 glVertexArrayId = GL_INVALID_INDEX;

	u32 trianglesCount = 0;

	RenderMesh(const MeshData& mesh) { Load(mesh); }
	~RenderMesh() { Free(); }

	void Draw() const
	{
		glBindVertexArray(glVertexArrayId);
		glDrawElements(GL_TRIANGLES, trianglesCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

private:

	void Load(const MeshData& mesh)
	{
		glGenBuffers(3, &glVerticesId);
		glGenVertexArrays(1, &glVertexArrayId);

		glBindVertexArray(glVertexArrayId);

		// Bind vertices VBO
		{
			glBindBuffer(GL_ARRAY_BUFFER, glVerticesId);
			const VertexBuffer& vertices = mesh.GetVertices();
			glBufferData(GL_ARRAY_BUFFER, vertices.Size() * sizeof(v3), vertices.Data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Bind normals VBO
		{
			glBindBuffer(GL_ARRAY_BUFFER, glNormalsId);
			const NormalsBuffer& normals = mesh.GetNormals();
			glBufferData(GL_ARRAY_BUFFER, normals.Size() * sizeof(v3), normals.Data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Bind triangles IBO
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glTrianglesId);
			const TriangleBuffer& triangles = mesh.GetTriangles();
			trianglesCount = triangles.Size() * 3;
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.Size() * sizeof(v3_u32), triangles.Data(), GL_STATIC_DRAW);
		}
		glBindVertexArray(0);
	}

	void Free()
	{
		// Free vertex, normals & triangles buffers
		glDeleteVertexArrays(1, &glVertexArrayId);
		glDeleteBuffers(3, &glVerticesId);
	}
};

