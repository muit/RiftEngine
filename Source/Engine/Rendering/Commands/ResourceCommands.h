// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "../RenderCommand.h"
#include "../TextureData.h"
#include "../MeshData.h"
#include "../Renderer.h"
#include "../Frame.h"


class LoadTextureCommand : public RenderCommand {
public:
	u32 id;
	TextureData texture;


	LoadTextureCommand(u32 id, TextureData texture) : id(id), texture{eastl::move(texture)} {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		render.resources.Load(id, eastl::move(texture));
	}
};

class FreeTextureCommand : public RenderCommand {
public:
	u32 id;


	FreeTextureCommand(u32 id) : id(id) {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		render.resources.Free<ResourceType::Texture>(id);
	}
};

class DrawTextureCommand : public RenderCommand {
public:
	u32 id;
	v2_u32 position;


	DrawTextureCommand(u32 id, v2_u32 position) : id{id}, position{position} {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		const TextureData& texture = render.resources.Get<ResourceType::Texture>(id);

		render.DrawImage(position, texture);
	}
};


class LoadMeshCommand : public RenderCommand {
public:
	u32 id;
	MeshData mesh;


	LoadMeshCommand(u32 id, MeshData texture) : id(id), mesh{ eastl::move(mesh) } {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		render.resources.Load(id, eastl::move(mesh));
	}
};

class FreeMeshCommand : public RenderCommand {
public:
	u32 id;


	FreeMeshCommand(u32 id) : id(id) {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		render.resources.Free<ResourceType::Mesh>(id);
	}
};

class DrawMeshCommand : public RenderCommand {
public:
	u32 id;
	Transform transform;

	DrawMeshCommand(u32 id, Transform transform) : id(id), transform(transform) {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		const MeshData& mesh = render.resources.Get<ResourceType::Mesh>(id);

		// Vertices over which we will operate
		MeshData::VertexBuffer vertices{ mesh.GetVertices() };
		MeshData::IndexBuffer triangles{ mesh.GetTriangles() };

		TransformToWorld(vertices);
		TransformToCamera(vertices);
		TransformToScreen(vertices);
		BackFaceCulling(vertices, triangles);

		Draw(vertices, triangles, Color::Cyan);
	}

private:

	void TransformToWorld(MeshData::VertexBuffer& vertices);
	void TransformToCamera(MeshData::VertexBuffer& vertices);
	void TransformToScreen(MeshData::VertexBuffer& vertices);
	void BackFaceCulling(MeshData::VertexBuffer& vertices, MeshData::IndexBuffer& triangles);

	void Draw(MeshData::VertexBuffer& vertices, MeshData::IndexBuffer& triangles, Color color);
};
