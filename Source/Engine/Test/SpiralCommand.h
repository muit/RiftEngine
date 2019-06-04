// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Tools/Profiler.h"
#include "Rendering/Frame.h"

#include "Rendering/Commands/RenderCommand.h"


class LoadSpiralCommand : public RenderCommand
{
public:
	u32 verticesCount;


	LoadSpiralCommand(u32 verticesCount) : verticesCount(verticesCount) {}
	virtual void Execute(FrameRender& render, Frame& frame) override
	{
		ScopedGraphicsZone("Load Spiral Command");
		render.resources.spiral = RenderSpiral(verticesCount);
	}
};

class DrawSpiralCommand : public RenderCommand
{
	AssetInfo material;

public:

	DrawSpiralCommand(AssetInfo material) : material(material) {}

	virtual void Execute(FrameRender& render, Frame& frame) override
	{
		ScopedGraphicsZone("Draw Spiral Command");

		const RenderMaterial& materialRes = render.resources.Get<ResourceType::Material>(material.GetPath());
		materialRes.Use();


		static const Name mvpParam{ "mvp" };

		const Matrix4f view = render.camera.GetViewMatrix();
		const Matrix4f projection{ render.Camera().GetProjectionMatrix(render.GetRenderSize()) };
		const Matrix4f mvp{ projection * view };
		materialRes.SetMatrix4f(mvpParam, mvp);

		render.resources.spiral.Draw();
	}
};
