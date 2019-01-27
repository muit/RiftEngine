// © 2019 Miguel Fernández Arce - All rights reserved

#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Core/World.h"
#include "Core/Assets/AssetInfo.h"
#include "Rendering/Commands/ResourceCommands.h"


bool Model::PostLoad()
{
	Super::PostLoad();

	data = {};

	Path rawModelPath = FileSystem::FindRawFile(FileSystem::FromString(Info().GetSPath()));
	if (!rawModelPath.empty())
	{
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(FileSystem::ToString(rawModelPath).c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			Log::Warning("ASSIMP: %s", importer.GetErrorString());
			return false;
		}

		data.FromAssimpScene(scene);

		GetWorld()->QueueRender<LoadMeshCommand>(0, data);
		return true;
	}

	return false;
}
