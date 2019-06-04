// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"


class CRotatingCube : public Component {
	STRUCT(CRotatingCube, Component)

	PROP(u8, segment, DetailsEdit)
	u8 segment;

	PROP(float, distanceFromCorner, DetailsEdit)
	float distanceFromCorner;

	PROP(float, speed, DetailsEdit)
	float speed = 4.f;
};

class CEj3Cube : public Component {
	STRUCT(CEj3Cube, Component)
};
