// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"


class CCamera : public Component {
	STRUCT(CCamera, Component)

	PROP(float, fov, DetailsEdit)
	float fov = 60.f;
};
