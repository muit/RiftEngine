// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "../RenderCommand.h"
#include "../TextureData.h"
#include "../MeshData.h"
#include "../Renderer.h"


class LoadTextureCommand : public RenderCommand {
public:
	u32 id;
	TextureData texture;

	virtual void Execute(Renderer& renderer) override {
		renderer.resources.Load(id, eastl::move(texture));
	}
};

class FreeTextureCommand : public RenderCommand {
public:
	u32 id;

	virtual void Execute(Renderer& renderer) override {
		renderer.resources.Free<ResourceType::Texture>(id);
	}
};

class DrawTextureCommand : public RenderCommand {
public:
	u32 id;

	virtual void Execute(Renderer& renderer) override {
		const TextureData& texture = renderer.resources.Get<ResourceType::Texture>(id);
	}
};


class LoadMeshCommand : public RenderCommand {
public:
	u32 id;
	MeshData mesh;

	virtual void Execute(Renderer& renderer) override {
		renderer.resources.Load(id, eastl::move(mesh));
	}
};

class FreeMeshCommand : public RenderCommand {
public:
	u32 id;

	virtual void Execute(Renderer& renderer) override {
		renderer.resources.Free<ResourceType::Mesh>(id);
	}
};

class DrawMeshCommand : public RenderCommand {
public:
	u32 id;

	virtual void Execute(Renderer& renderer) override {
		const MeshData& mesh = renderer.resources.Get<ResourceType::Mesh>(id);
	}
};
