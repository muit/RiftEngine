// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include <EASTL/unordered_map.h>

#include "Core/Strings/Name.h"
#include "Interface/Resources/RenderTexture.h"
#include "Interface/Resources/RenderMesh.h"
#include "Interface/Resources/RenderMaterial.h"


enum class ResourceType : u8 {
	Texture,
	Mesh,
	Material
};


struct Resources {

private:

	using TextureMap  = eastl::unordered_map<Name, RenderTexture>;
	using MeshMap     = eastl::unordered_map<Name, RenderMesh>;
	using MaterialMap = eastl::unordered_map<Name, RenderMaterial>;

	TextureMap textures;
	MeshMap meshes;
	MaterialMap materials;


public:

	void Load(Name id, const TextureData& data)
	{
		textures.insert_or_assign(id, RenderTexture{ data });
	}

	void Load(Name id, const MeshData& data)
	{
		meshes.insert_or_assign(id, RenderMesh{ data });
	}

	void Load(Name id, const MaterialData& data)
	{
		// #TODO: Parse Fragment / Vertex shader
		materials.insert_or_assign(id, RenderMaterial{ data });
	}

	template<ResourceType type>
	void Free(Name id)
	{
		if constexpr (type == ResourceType::Texture)
		{
			textures.erase(id);
		}
		else if constexpr(type == ResourceType::Mesh)
		{
			meshes.erase(id);
		}
		else if constexpr(type == ResourceType::Material)
		{
			materials.erase(id);
		}
	}

	template<ResourceType type>
	auto Get(Name id) -> eastl::enable_if_t<type == ResourceType::Texture, const RenderTexture&>
	{
		const TextureMap::const_iterator it = textures.find(id);
		assert(it != textures.end() && "Tried to access an unloaded resource.");
		return it->second;
	}

	template<ResourceType type>
	auto Get(Name id) -> eastl::enable_if_t<type == ResourceType::Mesh, const RenderMesh&>
	{
		const MeshMap::const_iterator it = meshes.find(id);
		assert(it != meshes.end() && "Tried to access an unloaded resource.");
		return it->second;
	}

	template<ResourceType type>
	auto Get(Name id) -> eastl::enable_if_t<type == ResourceType::Material, const RenderMaterial&>
	{
		const MaterialMap::const_iterator it = materials.find(id);
		assert(it != materials.end() && "Tried to access an unloaded resource.");
		return it->second;
	}
};
