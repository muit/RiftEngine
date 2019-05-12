// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "Core/Math/Color.h"


class CPointLight : public Component {
	STRUCT(CPointLight, Component)

	PROP(Color, color, DetailsEdit)
	Color color { Color::White };

	PROP(float, radius, DetailsEdit)
	float radius = 3.f;

	PROP(float, intensity, DetailsEdit)
	float intensity = 1.f;

	CPointLight() : Super(), color{ Color::White }, radius{2.f}, intensity{1.f} {}
};
