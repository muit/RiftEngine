// � 2019 Miguel Fern�ndez Arce - All rights reserved

#include "LightData.h"
#include "../Interface/Resources/RenderMaterial.h"


void LightingRender::Bind(const RenderMaterial& material)
{
	const i32 maxDirs = Math::Min(directionals.Size(), DirectionalLightData::maxLights);
	const i32 maxPoints = Math::Min(points.Size(), PointLightData::maxLights);

	// Ambient
	static const Name ambientName{ "ambientColor" };
	material.SetV3(ambientName, ambient.ToV3());

	// Directionals
	for (i32 i = 0; i < maxDirs; ++i)
	{
		const DirectionalLightData& light = directionals[i];

		const Name directionName{ CString::Printf("dir_lights[%i].direction", i) };
		material.SetV3(directionName, light.rotation.GetForward());

		const Name colorName{ CString::Printf("dir_lights[%i].color", i) };
		material.SetV3(colorName, light.color.ToV3() * light.intensity);
	}
	material.SetI32("num_dir_lights", maxDirs);


	// Points
	for (i32 i = 0; i < maxPoints; ++i)
	{
		const PointLightData& light = points[i];

		const Name positionName{ CString::Printf("point_lights[%i].position", i) };
		material.SetV3(positionName, light.position);

		const Name colorName{ CString::Printf("point_lights[%i].color", i) };
		material.SetV3(colorName, light.color.ToV3() * light.intensity);
	}
	material.SetI32("num_point_lights", maxPoints);
}
