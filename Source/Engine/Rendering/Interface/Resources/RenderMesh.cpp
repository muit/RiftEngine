// © 2019 Miguel Fernández Arce - All rights reserved

#include "RenderMesh.h"

void RenderMesh::Draw() const
{
	glBindVertexArray(glVertexArrayId);
	glDrawElements(GL_TRIANGLES, trianglesCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void RenderMesh::Load(const MeshData& mesh)
{
	glGenVertexArrays(1, &glVertexArrayId); // VAO
	glGenBuffers(2, &glVerticesId);

	// Bind VAO
	glBindVertexArray(glVertexArrayId);

	// Bind vertices (VBO)
	{
		const VertexBuffer& vertices = mesh.GetVertices();

		glBindBuffer(GL_ARRAY_BUFFER, glVerticesId);
		glBufferData(GL_ARRAY_BUFFER, vertices.Size() * sizeof(Vertex), vertices.Data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		// Stats
		verticesCount = vertices.Size();
	}

	// Bind triangles (EBO)
	{
		const TriangleBuffer& triangles = mesh.GetTriangles();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glTrianglesId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.Size() * sizeof(v3_u32), triangles.Data(), GL_STATIC_DRAW);

		// Stats
		trianglesCount = triangles.Size();
	}

	glBindVertexArray(0);
}

void RenderMesh::Free()
{
	// Free vertex, normals & triangles buffers
	glDeleteVertexArrays(1, &glVertexArrayId);
	glDeleteBuffers(2, &glVerticesId);
}
