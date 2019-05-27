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

	const Matrix4f view{ render.Camera().GetViewMatrix() };
	const Matrix4f projection{ render.Camera().GetProjectionMatrix(render.GetRenderSize()) };
	const Matrix4f vpMatrix{ projection * view };

	// Find resources
	const RenderMaterial& materialRes = render.resources.Get<ResourceType::Material>(material.GetPath());
	const RenderCubeTexture& textureRes = render.resources.Get<ResourceType::CubeTexture>(texture.GetPath());

	glDepthFunc(GL_LEQUAL);
	materialRes.Use();
	materialRes.SetMatrix4f(vpParameter, vpMatrix);

	textureRes.DrawAsSkybox();

	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}
