// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include "Core/MultiThreading.h"
#include "Core/Assets/AssetPtr.h"
#include "Assets/Texture.h"
#include "Assets/Mesh.h"
#include "RenderCommand.h"
#include "../Renderer.h"
#include "../Frame.h"


class LoadMeshCommand : public RenderCommand {
public:
	TAssetPtr<Mesh> asset;


	LoadMeshCommand(TAssetPtr<Mesh> asset) : asset{ asset } {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		render.resources.Load(asset.GetPath(), asset->GetMeshData());
	}
};

class FreeMeshCommand : public RenderCommand {
public:
	AssetInfo asset;


	FreeMeshCommand(AssetInfo asset) : asset{ asset } {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		render.resources.Free<ResourceType::Mesh>(asset.GetPath());
	}
};

class DrawMeshesCommand : public RenderCommand {
public:
	TArray<AssetInfo> meshes;
	TArray<Transform> transforms;


	DrawMeshesCommand(TArray<AssetInfo> meshes, TArray<Transform> transforms)
		: meshes{ MoveTemp(meshes) }
		, transforms{ MoveTemp(transforms) }
	{}

	virtual void Execute(FrameRender& render, Frame& frame) override;

private:

	/*SubTaskLambda VertexToWorld(VertexBuffer& vertices);
	SubTaskLambda NormalToWorld(NormalsBuffer& normals);
	TaskLambda    OperateVertexShader(FrameRender& render, const VertexBuffer& worldVertices, const NormalsBuffer& normals, LColorBuffer& colors);
	SubTaskLambda TransformToScreen(const FrameRender& render, const VertexBuffer& worldVertices, VertexBufferI32& screenVertices);
	TaskLambda    BackfaceCulling(const VertexBufferI32& vertices, TriangleBuffer& triangles);

	void RenderTriangles(FrameRender& render, const VertexBufferI32& vertices, const TriangleBuffer& triangles, const LColorBuffer& colors);*/
};
