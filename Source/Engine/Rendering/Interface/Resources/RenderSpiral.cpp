// © 2019 Miguel Fernández Arce - All rights reserved

#include "RenderSpiral.h"
#include "../OpenGL.h"
#include "Core/Log.h"


void RenderSpiral::Load(u32 length)
{
	glGenVertexArrays(1, &glVertexArrayId); // VAO
	glBindVertexArray(glVertexArrayId);

	glGenBuffers(1, &glVerticesId);       // VBO
	glGenBuffers(1, &glIndicesId);        // EBO
	glCheckError();


	// Store the fraction of a loop depending on length
	float angleIncrement = (2.f * Math::PI * 3.f) / length;
	float lastAngle = 0.f;
	TArray<u32> indices;
	TArray<float> vertexAngles;
	indices.Reserve(length);
	vertexAngles.Reserve(length);
	for (u32 i = 0; i < length; ++i)
	{
		indices.Add(i);
		vertexAngles.Add(lastAngle);
		lastAngle += angleIncrement;
	}

	// Bind vertices (VBO)
	{
		glBindBuffer(GL_ARRAY_BUFFER, glVerticesId);
		glBufferData(GL_ARRAY_BUFFER, vertexAngles.Size() * sizeof(float), vertexAngles.Data(), GL_STATIC_DRAW);

		// Angles
		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)nullptr);
		glEnableVertexAttribArray(0);

		// Stats
		verticesCount = vertexAngles.Size();

		glCheckError();
	}

	// Bind indices (EBO)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glIndicesId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(u32), indices.Data(), GL_STATIC_DRAW);

		// Stats
		indicesCount = length;

		glCheckError();
	}

	glBindVertexArray(0);

	glCheckError();

	Log::Message("Loaded spiral into GPU");
}

void RenderSpiral::Free()
{
	// Free vertex, normals & triangles buffers
	glDeleteVertexArrays(1, &glVertexArrayId);
	glDeleteBuffers(1, &glVerticesId);
	glDeleteBuffers(1, &glIndicesId);
}
