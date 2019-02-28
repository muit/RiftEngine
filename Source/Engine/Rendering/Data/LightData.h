// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"
#include "Core/Containers/Array.h"


struct DirectionalLightData {
	LinearColor color;
	float intensity;
	Quat rotation;

	DirectionalLightData() = default;
};

struct PointLightData {
	LinearColor color;
	float intensity;
	float radius;
	v3 position;

	PointLightData() = default;
};


struct LightingRender {
	TArray<DirectionalLightData> directionals;
	TArray<PointLightData> points;

	LightingRender() = default;
};
