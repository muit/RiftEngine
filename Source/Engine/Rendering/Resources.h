// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include "Core/Containers/Map.h"

#include "Core/Strings/Name.h"
#include "Interface/Resources/RenderTexture.h"
#include "Interface/Resources/RenderMesh.h"
#include "Interface/Resources/RenderMaterial.h"
#include "Interface/Resources/RenderCubeTexture.h"


enum class ResourceType : u8 {
	Texture,
	Mesh,
	Material,
	CubeTexture
};


struct Resources {

private:

	using TextureMap     = TMap<Name, RenderTexture>;
	using MeshMap        = TMap<Name, RenderMesh>;
	using MaterialMap    = TMap<Name, RenderMaterial>;
	using CubeTextureMap = TMap<Name, RenderCubeTexture>;

	TextureMap textures;
	MeshMap meshes;
	MaterialMap materials;
	CubeTextureMap cubeTextures;


public:

	Resources() {
		textures.SetEmptyKey(Name::None());
		meshes.SetEmptyKey(Name::None());
		materials.SetEmptyKey(Name::None());
		cubeTextures.SetEmptyKey(Name::None());
	}

	void Load(Name id, const TextureData& data)
	{
		textures.Insert(id, RenderTexture{ data });
	}

	void Load(Name id, const MeshData& data)
	{
		meshes.Insert(id, RenderMesh{ id, data });
	}

	void Load(Name id, const MaterialData& data)
	{
		materials.Insert(id, RenderMaterial{ id, data });
	}

	void Load(Name id, const CubeTextureData& data)
	{
		cubeTextures.Insert(id, RenderCubeTexture{ data });
	}

	template<ResourceType type>
	void Free(Name id)
	{
		if constexpr (type == ResourceType::Texture)
		{
			textures.Remove(id);
		}
		else if constexpr(type == ResourceType::Mesh)
		{
			meshes.Remove(id);
		}
		else if constexpr(type == ResourceType::Material)
		{
			materials.Remove(id);
		}
		else if constexpr(type == ResourceType::CubeTexture)
		{
			cubeTextures.Remove(id);
		}
	}

	template<ResourceType type>
	auto Get(Name id) const -> eastl::enable_if_t<type == ResourceType::Texture, const RenderTexture&>
	{
		auto* resource = textures.Find(id);
		assert(resource && "Tried to access an unloaded resource.");
		return *resource;
	}

	template<ResourceType type>
	auto Get(Name id) const -> eastl::enable_if_t<type == ResourceType::Mesh, const RenderMesh&>
	{
		auto* resource = meshes.Find(id);
		assert(resource && "Tried to access an unloaded resource.");
		return *resource;
	}

	template<ResourceType type>
	auto Get(Name id) const -> eastl::enable_if_t<type == ResourceType::Material, const RenderMaterial&>
	{
		auto* resource = materials.Find(id);
		assert(resource && "Tried to access an unloaded resource.");
		return *resource;
	}

	template<ResourceType type>
	auto Get(Name id) const -> eastl::enable_if_t<type == ResourceType::CubeTexture, const RenderCubeTexture&>
	{
		auto* resource = cubeTextures.Find(id);
		assert(resource && "Tried to access an unloaded resource.");
		return *resource;
	}
};
