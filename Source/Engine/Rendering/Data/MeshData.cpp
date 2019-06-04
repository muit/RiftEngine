// © 2019 Miguel Fernández Arce - All rights reserved

#include "MeshData.h"
#include <assimp/scene.h>
#include <assimp/mesh.h>

#include "Core/Log.h"
#include "Tools/Profiler.h"
#include "Assets/Mesh.h"


void MeshData::FromAssimpScene(const aiScene* scene, StaticMesh& asset)
{
	if (scene)
	{
		ScopedGameZone("Parse mesh data");

		vertices.Empty(false);
		triangles.Empty(false);

		ProcessNode(scene->mRootNode, scene, asset);
	}
}

void MeshData::ProcessNode(aiNode *node, const aiScene *scene, StaticMesh& asset)
{
	// Process all the node's meshes (if any)
	for (u32 i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene, asset);
	}

	// Then iterate children
	for (u32 i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene, asset);
	}
}

void MeshData::ProcessMesh(aiMesh *mesh, const aiScene* scene, StaticMesh& asset)
{
	const float importScale = asset.importScale;

	// Starting vertex index of this mesh
	const v3_u32 vertexIndexOffset = v3_u32{ (u32)vertices.Size() };

	// Process Vertices
	vertices.Reserve(mesh->mNumVertices);

	// If have any UV channel
	if (mesh->GetNumUVChannels() > 0)
	{
		for (u32 i = 0; i < mesh->mNumVertices; ++i)
		{
			const aiVector3D& position = mesh->mVertices[i];
			const aiVector3D& normal = mesh->mNormals[i];
			const aiVector3D& uv = mesh->mTextureCoords[0][i];

			vertices.Add(Vertex {
				v3{ position.x, position.y, position.z } * importScale,
				v3{ normal.x, normal.y, normal.z },
				v2{ uv.x, uv.y }
			});
		}
	}
	else
	{
		for (u32 i = 0; i < mesh->mNumVertices; ++i)
		{
			const aiVector3D& position = mesh->mVertices[i];
			const aiVector3D& normal = mesh->mNormals[i];

			vertices.Add(Vertex{
				v3{ position.x, position.y, position.z } * importScale,
				v3{ normal.x, normal.y, normal.z },
				v2::Zero()
			});
		}
	}

	// Process Triangles
	triangles.Reserve(mesh->mNumFaces);
	for (u32 i = 0; i < mesh->mNumFaces; ++i)
	{
		const aiFace& face = mesh->mFaces[i];

		if(face.mNumIndices == 3)
		{
			const v3_u32* triangleIndex = reinterpret_cast<const v3_u32*>(face.mIndices);
			triangles.Add(*triangleIndex + vertexIndexOffset);
		}
		else
		{
			Log::Warning("Mesh face had %i vertices. Its not a triangle", face.mNumIndices);
		}
	}
}
