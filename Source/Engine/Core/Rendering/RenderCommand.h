// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"


/**
 * Commands get scheduled on Game thread and executed sequentially on Render thread.
 * They contain the state of the render data no matter which frame is being rendered.
 */
class RenderCommand {
public:

	virtual ~RenderCommand() {}

	virtual void Execute(class Renderer& renderer, struct Frame& frame) = 0;
};
