// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"


class CGlobalCamera : public Component {
	STRUCT(CGlobalCamera, Component)

	PROP(Transform, transform)
	Transform transform;
};
