// © 2019 Miguel Fernández Arce - All rights reserved

#include "RenderMesh.h"
#include "../OpenGL.h"
#include "Core/Log.h"


void RenderMesh::Load(Name id, const MeshData& mesh)
{
	glGenVertexArrays(1, &glVertexArrayId); // VAO
	glBindVertexArray(glVertexArrayId);

	glGenBuffers(1, &glVerticesId);         // VBO
	glGenBuffers(1, &glTrianglesId);        // EBO
	glCheckError();

	// Bind vertices (VBO)
	{
		const VertexBuffer& vertices = mesh.GetVertices();

		glBindBuffer(GL_ARRAY_BUFFER, glVerticesId);
		glBufferData(GL_ARRAY_BUFFER, vertices.Size() * sizeof(Vertex), vertices.Data(), GL_STATIC_DRAW);

		// Positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);
		glEnableVertexAttribArray(0);

		// Normals
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);

		// UVs
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glEnableVertexAttribArray(2);

		// Stats
		verticesCount = vertices.Size();

		glCheckError();
	}

	// Bind triangles (EBO)
	{
		const TriangleBuffer& triangles = mesh.GetTriangles();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glTrianglesId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.Size() * sizeof(Triangle), triangles.Data(), GL_STATIC_DRAW);

		// Stats
		indicesCount = triangles.Size() * 3;

		glCheckError();
	}

	glBindVertexArray(0);

	glCheckError();

	Log::Message("Loaded mesh into GPU '%s' Id:%i", id.ToString().c_str(), glVertexArrayId);
}

void RenderMesh::Free()
{
	// Free vertex, normals & triangles buffers
	glDeleteVertexArrays(1, &glVertexArrayId);
	glDeleteBuffers(1, &glVerticesId);
	glDeleteBuffers(1, &glTrianglesId);
}
