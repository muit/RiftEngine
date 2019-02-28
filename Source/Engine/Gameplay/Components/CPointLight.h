// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"


class CPointLight : public Component {
	STRUCT(CPointLight, Component)

	PROP(Color, color)
	Color color;

	PROP(float, radius)
	float radius;

	PROP(float, intensity)
	float intensity;

	CPointLight() : Super(), color{ Color::White }, radius{2.f}, intensity{1.f} {}
};
