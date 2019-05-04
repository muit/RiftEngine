// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"


class CPhysicsWorld : public Component {
	STRUCT(CPhysicsWorld, Component)
public:

	PROP(v2, gravity)
	v2 gravity {0.0f, -10.f};

	// Used by all colliders without a CBody2D. They are threated as Static
	Body2D globalStaticBody;
};
