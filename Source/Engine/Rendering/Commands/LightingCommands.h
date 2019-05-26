// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Tools/Profiler.h"
#include "../Data/LightData.h"
#include "../Renderer.h"
#include "../Frame.h"

#include "RenderCommand.h"

class DrawAmbientLightCommand : public RenderCommand {
public:
	LinearColor color;

	DrawAmbientLightCommand(LinearColor color) : color(color) {}
	virtual void Execute(FrameRender& render, Frame& frame) override
	{
		ScopedGraphicsZone("Ambient Light Command");
		render.lighting.ambient = color;
	}
};

class DrawDirectionalLightCommand : public RenderCommand {
public:
	TArray<DirectionalLightData> lights;

	DrawDirectionalLightCommand(TArray<DirectionalLightData>&& lights)
		: lights(lights)
	{}
	virtual void Execute(FrameRender& render, Frame& frame) override
	{
		ScopedGraphicsZone("Directional Lights Command");
		render.lighting.directionals.Append(MoveTemp(lights));
	}
};

class DrawPointLightCommand : public RenderCommand {
public:
	TArray<PointLightData> lights;

	DrawPointLightCommand(TArray<PointLightData>&& lights)
		: lights(lights)
	{}
	virtual void Execute(FrameRender& render, Frame& frame) override
	{
		ScopedGraphicsZone("Point Lights Command");
		render.lighting.points.Append(MoveTemp(lights));
	}
};
