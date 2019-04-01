// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include <EASTL/unordered_map.h>

#include "Core/Strings/Name.h"
#include "Data/TextureData.h"
#include "Data/MeshData.h"
#include "Interface/Resources/RenderTexture.h"
#include "Interface/Resources/RenderMesh.h"


enum class ResourceType : u8 {
	Texture,
	Mesh
};


struct Resources {

private:

	using TextureMap = eastl::unordered_map<Name, RenderTexture>;
	using MeshMap    = eastl::unordered_map<Name, RenderMesh>;

	TextureMap textures;
	MeshMap meshes;


public:

	void Load(Name id, const TextureData& data)
	{
		textures.insert_or_assign(id, RenderTexture{ data });
	}

	void Load(Name id, const MeshData& data)
	{
		meshes.insert_or_assign(id, RenderMesh{ data });
	}

	template<ResourceType type>
	void Free(Name id)
	{
		if constexpr (type == ResourceType::Texture)
		{
			auto& idIterator = textures.find(id);
			if (idIterator != textures.end())
			{
				GLTextures::Free(idIterator->second);
				textures.erase(idIterator);
			}
		}
		else if(type == ResourceType::Mesh)
		{
			meshes_DEPRECATED.erase(id);
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
