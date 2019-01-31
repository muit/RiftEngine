// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include <EASTL/unordered_map.h>

#include "Data/TextureData.h"
#include "Data/MeshData.h"


enum class ResourceType : u8 {
	Texture,
	Mesh
};


struct Resources {

private:

	using TextureMap = eastl::unordered_map<u32, TextureData>;
	using MeshMap    = eastl::unordered_map<u32, MeshData>;

	TextureMap textures;
	MeshMap meshes;

public:

	void Load(u32 id, TextureData&& texture) {
		textures.insert_or_assign(id, texture);
	}

	void Load(u32 id, MeshData&& mesh) {
		meshes.insert_or_assign(id, mesh);
	}

	template<ResourceType type>
	void Free(u32 id) {
		switch(type)
		{
		case ResourceType::Texture:
			textures.erase(id);
			break;
		case ResourceType::Mesh:
			meshes.erase(id);
			break;
		}
	}

	template<ResourceType type>
	auto Get(u32 id) -> eastl::enable_if_t<type == ResourceType::Texture, const TextureData&>
	{
		TextureMap::const_iterator it = textures.find(id);
		assert(it != textures.end() && "Tried to access an unloaded resource.");
		return it->second;
	}

	template<ResourceType type>
	auto Get(u32 id) -> eastl::enable_if_t<type == ResourceType::Mesh, const MeshData&>
	{
		MeshMap::const_iterator it = meshes.find(id);
		assert(it != meshes.end() && "Tried to access an unloaded resource.");
		return it->second;
	}
};
