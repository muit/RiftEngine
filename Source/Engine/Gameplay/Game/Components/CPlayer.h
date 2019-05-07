// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"


class CPlayer : public Component {
	STRUCT(CPlayer, Component)

	PROP(float, impulse, DetailsEdit)
	float impulse = 4.f;

	PROP(float, dieHeight, DetailsEdit)
	float dieHeight = -2.f;

	PROP(float, cameraDistance, DetailsEdit)
	float cameraDistance = 4.f;

	PROP(float, cameraSpeed, DetailsEdit)
	float cameraSpeed = 4.f;

	PROP(v3, homeLocation, Transient)
	v3 homeLocation;
};
