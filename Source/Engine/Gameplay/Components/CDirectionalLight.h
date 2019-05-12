// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "Core/Math/Color.h"


class CDirectionalLight : public Component {
	STRUCT(CDirectionalLight, Component)

	PROP(Color, color, DetailsEdit)
	Color color { Color::White };

	PROP(float, intensity, DetailsEdit)
	float intensity = 1.f;

	CDirectionalLight() : Super(), color{Color::White}, intensity{ 1.f } {}
};
