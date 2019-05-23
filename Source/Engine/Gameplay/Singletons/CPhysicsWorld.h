// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"


class CPhysicsWorld : public Component {
	STRUCT(CPhysicsWorld, Component)
public:

	PROP(v3, gravity, DetailsEdit)
	v3 gravity {0.f, 0.f, -9.81f};
};
