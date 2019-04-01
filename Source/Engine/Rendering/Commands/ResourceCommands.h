// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include <taskflow/taskflow.hpp>

#include "Core/MultiThreading.h"
#include "Core/Assets/AssetPtr.h"
#include "Assets/Texture.h"
#include "Assets/Mesh.h"
#include "../RenderCommand.h"
#include "../Renderer.h"
#include "../Frame.h"
#include "../Rasterizer.h"


// #TODO: Commands should be able to receive many orders of the same type.
// E.g: Draw all this meshes


class LoadTextureCommand : public RenderCommand {
public:
	TAssetPtr<Texture> asset;


	LoadTextureCommand(TAssetPtr<Texture> asset) : asset{ asset } {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		// #TODO: Avoid texture data copy by adding dependency on the asset
		//render.resources.Load(id, texture);
	}
};

class FreeTextureCommand : public RenderCommand {
public:
	AssetInfo asset;


	FreeTextureCommand(AssetInfo asset) : asset{ asset } {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		//render.resources.Free<ResourceType::Texture>(asset);
	}
};

class Draw2DTextureCommand : public RenderCommand {
public:
	AssetInfo asset;
	v2_u32 position;


	Draw2DTextureCommand(AssetInfo asset, v2_u32 position)
		: asset{asset}, position{position}
	{}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		//u32 texture = render.resources.GetGLId<ResourceType::Texture>(id);

		//render.DrawImage(position, texture);
	}
};


class LoadMeshCommand : public RenderCommand {
public:
	TAssetPtr<Mesh> asset;


	LoadMeshCommand(TAssetPtr<Mesh> asset) : asset{ asset } {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		//render.resources.Load(asset, eastl::move(mesh));
	}
};

class FreeMeshCommand : public RenderCommand {
public:
	AssetInfo asset;


	FreeMeshCommand(AssetInfo asset) : asset{ asset } {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		//render.resources.Free<ResourceType::Mesh>(id);
	}
};

class DrawMeshCommand : public RenderCommand {
public:
	AssetInfo asset;
	Transform transform;
	Color color;


	DrawMeshCommand(AssetInfo asset) : asset{ asset } {}

	virtual void Execute(FrameRender& render, Frame& frame) override;

private:

	/*SubTaskLambda VertexToWorld(VertexBuffer& vertices);
	SubTaskLambda NormalToWorld(NormalsBuffer& normals);
	TaskLambda    OperateVertexShader(FrameRender& render, const VertexBuffer& worldVertices, const NormalsBuffer& normals, LColorBuffer& colors);
	SubTaskLambda TransformToScreen(const FrameRender& render, const VertexBuffer& worldVertices, VertexBufferI32& screenVertices);
	TaskLambda    BackfaceCulling(const VertexBufferI32& vertices, TriangleBuffer& triangles);

	void RenderTriangles(FrameRender& render, const VertexBufferI32& vertices, const TriangleBuffer& triangles, const LColorBuffer& colors);*/
};
