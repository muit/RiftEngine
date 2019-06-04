// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Tools/Profiler.h"

#include "Core/MultiThreading.h"
#include "Core/Assets/AssetPtr.h"
#include "Assets/Mesh.h"
#include "../Renderer.h"
#include "../Frame.h"

#include "RenderCommand.h"


class LoadMeshCommand : public RenderCommand {
public:
	AssetInfo asset;
	const MeshData* dataPtr;

	LoadMeshCommand(TAssetPtr<Mesh> asset) : asset{ asset.GetInfo() }, dataPtr{ &asset->GetMeshData() } {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		ScopedGraphicsZone("Load Mesh Command");
		render.resources.Load(asset.GetPath(), *dataPtr);
	}
};

class FreeMeshCommand : public RenderCommand {
public:
	AssetInfo asset;


	FreeMeshCommand(AssetInfo asset) : asset{ asset } {}

	virtual void Execute(FrameRender& render, Frame& frame) override {
		ScopedGraphicsZone("Free Mesh Command");
		render.resources.Free<ResourceType::Mesh>(asset.GetPath());
	}
};


struct MeshDrawInstance
{
	AssetInfo mesh;
	AssetInfo material;
	Transform transform;
};

class DrawMeshesCommand : public RenderCommand {

	TArray<MeshDrawInstance> meshes;

public:

	DrawMeshesCommand(TArray<MeshDrawInstance> meshes)
		: meshes{ MoveTemp(meshes) }
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
