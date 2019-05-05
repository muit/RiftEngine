// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "../Singletons/CVisualDebugger.h"


class SRenderVisualDebugger : public System {
	CLASS(SRenderVisualDebugger, System)

	using VDFramePtr = eastl::unique_ptr<VisualDebuggerFrame>;

	// Rendered visual queue
	VDFramePtr renderQueue;

	CVisualDebugger* visualDebugger;


public:

	SRenderVisualDebugger()
		: Super()
		, renderQueue{ eastl::make_unique<VisualDebuggerFrame>() }
		, visualDebugger{nullptr}
	{
		bTickOnEditor = true;
	}

	virtual void BeginPlay() override;
	virtual void Tick(float /*deltaTime*/) override;
	virtual void BeforeDestroy() override;
};
