// © 2019 Miguel Fernández Arce - All rights reserved

#include "MeshData.h"
#include <assimp/scene.h>
#include <assimp/mesh.h>

#include "Core/Log.h"


void MeshData::FromAssimpScene(const aiScene* scene)
{
	if (scene)
	{
		vertices.Empty();
		normals.Empty();
		triangles.Empty();

		ProcessNode(scene->mRootNode, scene);
	}
}

void MeshData::ProcessNode(aiNode *node, const aiScene *scene)
{
	// Process all the node's meshes (if any)
	for (u32 i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}

	// Then iterate children
	for (u32 i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void MeshData::ProcessMesh(aiMesh *mesh, const aiScene* scene)
{
	const u32 lastVertexIndex = (u32)vertices.Size();

	// Process Vertices
	vertices.Reserve(mesh->mNumVertices);
	normals.Reserve(mesh->mNumVertices);
	for (u32 i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.Add({
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		});

		normals.Add({
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		});
	}

	// Process Triangles
	vertices.Reserve(mesh->mNumVertices);
	for (u32 i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];

		if (face.mNumIndices == 3)
		{
			triangles.Add({
				face.mIndices[0] + lastVertexIndex,
				face.mIndices[1] + lastVertexIndex,
				face.mIndices[2] + lastVertexIndex
			});
		}
		else
		{
			Log::Warning("Mesh face had %i vertices. Its not a triangle", face.mNumIndices);
		}
	}
}
