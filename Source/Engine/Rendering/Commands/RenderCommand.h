// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Core/Math/Transform.h"
#include "../Data/CameraData.h"


/**
 * Commands get scheduled on Game thread and executed sequentially on Render thread.
 * They contain the state of the render data no matter which frame is being rendered.
 */
class RenderCommand {
public:
	virtual ~RenderCommand() {}

	virtual void Execute(struct FrameRender& render, struct Frame& frame) = 0;
};


class CameraCommand : public RenderCommand {

	CameraData camera;


public:

	CameraCommand(CameraData&& camera) : camera{camera} {}

	virtual void Execute(struct FrameRender& render, Frame& frame) override;
};
