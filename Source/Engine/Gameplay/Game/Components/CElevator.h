// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"


class CElevator : public Component {
	STRUCT(CElevator, Component)

	PROP(Name, keyId, DetailsEdit)
	Name keyId;

	PROP(bool, bEnabled, DetailsView)
	bool bEnabled;

	PROP(v3, firstPosition, DetailsEdit)
	v3 firstPosition;

	PROP(v3, secondPosition, DetailsEdit)
	v3 secondPosition;

	PROP(float, speed, DetailsEdit)
	float speed = 4.f;

	PROP(bool, bReturning)
	bool bReturning = false;
};
