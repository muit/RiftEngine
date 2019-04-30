// © 2019 Miguel Fernández Arce - All rights reserved

#include "Texture.h"
#include <SDL_surface.h>
#include <SDL_image.h>

#include "Core/Engine.h"
#include "Core/Assets/AssetPtr.h"
#include "Core/Files/FileSystem.h"
#include "Rendering/Commands/TextureCommands.h"


bool Texture::PostLoad()
{
	data = {};

	Path rawTexturePath = FileSystem::FindRawFile(FileSystem::FromString(GetInfo().GetSPath()));
	if(!rawTexturePath.empty())
	{
		SDL_Surface* rawImg = IMG_Load(FileSystem::ToString(rawTexturePath).c_str());
		// Copy texture data
		data.FromSurface(rawImg);
		SDL_FreeSurface(rawImg);

		QueueRenderCommand<LoadTextureCommand>(TAssetPtr<Texture>{ Self<Texture>() });

		return Super::PostLoad();;
	}
	return false;
}

void Texture::OnUnload()
{
	QueueRenderCommand<FreeTextureCommand>(GetInfo());
}
