// © 2019 Miguel Fernández Arce - All rights reserved

#include "SkyboxCommand.h"

#include "Core/Engine.h"
#include "Core/Math/Vector.h"
#include "Tools/Profiler.h"
#include "tracy/TracyOpenGL.hpp"


using MeshTransformMap = TMap<Name, TArray<Transform>>;
using MaterialBatchMap = TMap<Name, MeshTransformMap>;


void DrawSkyboxCommand::Execute(FrameRender& render, Frame& frame)
{
	ScopedGraphicsZone("Draw Skybox Command");

	static const Name vpParameter { "vp" };

	const Matrix4f view{ render.Camera().GetViewMatrixNoLocation() };
	const Matrix4f projection{ render.Camera().GetProjectionMatrix(render.GetRenderSize()) };
	const Matrix4f vpMatrix{ projection * view };

	// Find resources
	const RenderMesh& cubeRes = render.resources.Get<ResourceType::Mesh>(cube.GetPath());
	const RenderMaterial& materialRes = render.resources.Get<ResourceType::Material>(material.GetPath());
	const RenderCubeTexture& textureRes = render.resources.Get<ResourceType::CubeTexture>(texture.GetPath());

	glCullFace(GL_FRONT);
	glDepthMask(GL_FALSE);

	materialRes.Use();
	materialRes.SetMatrix4f(vpParameter, vpMatrix);

	cubeRes.Bind();
	textureRes.Bind();

	cubeRes.Draw();

	RenderMesh::Unbind();

	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);
}
