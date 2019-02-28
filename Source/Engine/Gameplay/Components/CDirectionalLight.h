// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"


class CDirectionalLight : public Component {
	STRUCT(CDirectionalLight, Component)

	PROP(Color, color)
	Color color;

	PROP(float, intensity)
	float intensity;

	CDirectionalLight() : Super(), color{Color::White}, intensity{ 1.f } {}
};
