// © 2019 Miguel Fernández Arce - All rights reserved

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Core/Engine.h"
#include "Core/Assets/AssetPtr.h"
#include "Rendering/Commands/MeshCommands.h"
#include "../Tools/Profiler.h"


bool Mesh::PostLoad()
{
	data = {};

	Path rawModelPath = FileSystem::FindRawFile(FileSystem::FromString(GetInfo().GetSPath()));
	if (!rawModelPath.empty())
	{
		Assimp::Importer importer;
		const aiScene* scene;
		{
			ScopedZone("Load Mesh File", BB45D1);

			scene = importer.ReadFile(FileSystem::ToString(rawModelPath).c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
		}

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			Log::Warning("Could not load model: %s", importer.GetErrorString());
			return false;
		}

		data.FromAssimpScene(scene);

		QueueRenderCommand<LoadMeshCommand>(TAssetPtr<Mesh>{ Self().Cast<Mesh>() });

		// End by loading material
		material.Load();

		return Super::PostLoad();
	}

	return false;
}

void Mesh::OnUnload()
{
	QueueRenderCommand<FreeMeshCommand>(GetInfo());
}
