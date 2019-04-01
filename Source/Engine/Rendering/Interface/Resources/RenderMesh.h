// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "../../Data/MeshData.h"


struct RenderMesh
{
	u32 glId = GL_INVALID_INDEX;


	RenderMesh(const MeshData& mesh) { Load(mesh); }
	~RenderMesh() { Free(); }

	void Bind() {}

private:

	void Load(const MeshData& mesh)
	{ }

	void Free()
	{
		glDeleteTextures(1, &glId);
	}
};

