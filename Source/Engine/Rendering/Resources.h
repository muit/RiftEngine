// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include <EASTL/unordered_map.h>

#include "TextureData.h"
#include "MeshData.h"


enum class ResourceType : u8 {
	Texture,
	Mesh
};


struct Resources {

private:

	eastl::unordered_map<u32, TextureData> textures;
	eastl::unordered_map<u32, MeshData> meshes;

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
		return textures.at(id);
	}

	template<ResourceType type>
	auto Get(u32 id) -> eastl::enable_if_t<type == ResourceType::Mesh, const MeshData&>
	{
		return meshes.at(id);
	}
};
