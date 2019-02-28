// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"
#include "Core/Containers/Array.h"

struct BaseLightData {
	LinearColor color;
	float intensity;

	BaseLightData() = default;
};

using AmbientLightData = BaseLightData;

struct DirectionalLightData : public BaseLightData {
	Quat rotation;

	DirectionalLightData() = default;
};

struct PointLightData : public BaseLightData {
	float radius;
	v3 position;

	PointLightData() = default;
};


struct LightingRender {
	TArray<DirectionalLightData> directionals;
	TArray<PointLightData> points;

	LightingRender() = default;
};
