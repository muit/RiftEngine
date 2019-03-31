// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include <EASTL/unordered_map.h>

#include "Core/Misc/Optional.h"
#include "Data/TextureData.h"
#include "Data/MeshData.h"


enum class ResourceType : u8 {
	Texture,
	Mesh
};


struct Resources {

private:

	using TextureIdMap = eastl::unordered_map<u32, u32>;
	using MeshIdMap    = eastl::unordered_map<u32, u32>;
	using MeshMap_DEPRECATED = eastl::unordered_map<u32, MeshData>;

	TextureIdMap textures;
	MeshIdMap meshes;
	MeshMap_DEPRECATED meshes_DEPRECATED;

public:

	void Load(u32 id, const TextureData& data)
	{
		// Send data to opengl and store texture Id
		textures.insert_or_assign(id, GLTextures::Bind(data));
	}

	void Load(u32 id, MeshData&& data)
	{
		meshes_DEPRECATED.insert_or_assign(id, data);
	}

	template<ResourceType type>
	void Free(u32 id)
	{
		switch(type)
		{
		case ResourceType::Texture:
			auto& idIterator = textures.find(id);
			if (idIterator != textures.end())
			{
				GLTextures::Free(idIterator->second);
				textures.erase(idIterator);
			}
			break;
		case ResourceType::Mesh:
			meshes_DEPRECATED.erase(id);
			break;
		}
	}

	template<ResourceType type>
	auto GetGLId(u32 id) -> eastl::enable_if_t<type == ResourceType::Texture, u32>
	{
		const TextureIdMap::const_iterator it = textures.find(id);
		assert(it != textures.end() && "Tried to access an unloaded resource.");
		return it->second;
	}

	template<ResourceType type>
	auto Get(u32 id) -> eastl::enable_if_t<type == ResourceType::Mesh, const MeshData&>
	{
		const MeshMap_DEPRECATED::const_iterator it = meshes_DEPRECATED.find(id);
		assert(it != meshes_DEPRECATED.end() && "Tried to access an unloaded resource.");
		return it->second;
	}
};
