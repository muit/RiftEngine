// © 2019 Miguel Fernández Arce - All rights reserved

#include "Texture.h"
#include <SDL_surface.h>
#include <SDL_image.h>

#include "Core/Engine.h"
#include "Core/Assets/AssetPtr.h"
#include "Core/Files/FileSystem.h"
#include "Rendering/Commands/TextureCommands.h"


const TAssetPtr<Texture> Texture::default { "Textures/T_Default.png.meta" };

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

		data.filtering = ETextureFiltering(filtering);

		QueueRenderCommand<LoadTextureCommand>(TAssetPtr<Texture>{ Self<Texture>() });

		return Super::PostLoad();;
	}
	return false;
}

void Texture::OnUnload()
{
	QueueRenderCommand<FreeTextureCommand>(GetInfo());
}

const v2_u32& Texture::GetSize() const
{
	return data.Size();
}

const TextureData& Texture::GetTextureData() const
{
	return data;
}
