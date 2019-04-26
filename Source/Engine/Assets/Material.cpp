// © 2019 Miguel Fernández Arce - All rights reserved

#include "Texture.h"
#include <SDL_surface.h>
#include <SDL_image.h>

#include "Core/Engine.h"
#include "Core/Assets/AssetPtr.h"
#include "Core/Files/FileSystem.h"
#include "Rendering/Commands/MaterialCommands.h"


bool Material::PostLoad()
{
	data = {};

	Path rawTexturePath = FileSystem::FindRawFile(FileSystem::FromString(GetInfo().GetSPath()));
	if(!rawTexturePath.empty())
	{
		String code;
		if (!FileSystem::LoadStringFile(rawTexturePath, code))
		{
			// Shader file not found or loaded
			return false;
		}

		if (!data.Parse(code))
		{
			// Invalid code
			return false;
		}

		QueueRenderCommand<LoadMaterialCommand>(
			TAssetPtr<Material>{ Self<Material>() }
		);

		return Super::PostLoad();
	}
	return false;
}

void Material::OnUnload()
{
	QueueRenderCommand<FreeMaterialCommand>(GetInfo());
}
