// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "../../Data/TextureData.h"
#include "../OpenGL.h"


struct RenderSpiral
{
	u32 glVerticesId = GL_INVALID_INDEX;
	u32 glIndicesId = GL_INVALID_INDEX;

	u32 glVertexArrayId = GL_INVALID_INDEX;

	u32 indicesCount = 0;
	u32 verticesCount = 0;


	RenderSpiral() = default;
	RenderSpiral(u32 length) { Load(length); }
	RenderSpiral(RenderSpiral&& other) { MoveFrom(MoveTemp(other)); }
	RenderSpiral& operator=(RenderSpiral&& other) { MoveFrom(MoveTemp(other)); return *this; }
	~RenderSpiral() { Free(); }

	void Bind() const
	{
		glBindVertexArray(glVertexArrayId);
		glCheckError();
	}

	void Draw() const
	{
		glBindVertexArray(glVertexArrayId);
		glDrawElements(GL_LINE_STRIP, indicesCount, GL_UNSIGNED_INT, 0);
		glCheckError();
	}

	static void Unbind() { glBindVertexArray(0); }

private:

	void Load(u32 length);

	void Free();

	void MoveFrom(RenderSpiral&& other)
	{
		glVertexArrayId = other.glVertexArrayId;
		glVerticesId = other.glVerticesId;
		glIndicesId = other.glIndicesId;
		indicesCount = other.indicesCount;
		verticesCount = other.verticesCount;
		other.glVertexArrayId = GL_INVALID_INDEX;
		other.glVerticesId = GL_INVALID_INDEX;
		other.glIndicesId = GL_INVALID_INDEX;
		other.indicesCount = 0;
		other.glVerticesId = 0;
	}
};

