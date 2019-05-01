// Copyright 2015-2019 Piperift - All rights reserved

#include "SLighting.h"
#include "Core/Engine.h"
#include "Tools/Profiler.h"

#include "../Components/CEditorCamera.h"
#include "../Components/CDirectionalLight.h"
#include "../Components/CPointLight.h"
#include "Rendering/Commands/LightingCommands.h"


void SLighting::Tick(float deltaTime)
{
	ScopedStackGameZone();
	Super::Tick(deltaTime);

	CameraData cameraData{};

	TArray<DirectionalLightData> directionals {};
	TArray<PointLightData> points {};

	// Gather directionals data
	auto dirView = ECS()->View<CTransform, CDirectionalLight>();

	directionals.Reserve((i32)dirView.size());
	dirView.each([deltaTime, &directionals](EntityId e, CTransform& t, CDirectionalLight& light)
	{
		// Rotate the light
		Rotator r = t.transform.GetRotation();
		r.x += 10.f * deltaTime;
		r.z += 1.f * deltaTime;
		t.transform.SetRotation(r);

		directionals.Add({
			LinearColor{ light.color },
			light.intensity,
			t.transform.rotation
		});
	});

	// Gather points data
	auto pointView = ECS()->View<CTransform, CPointLight>();

	points.Reserve((i32)pointView.size());
	pointView.each([&points](EntityId e, CTransform& t, CPointLight& light)
	{
		points.Add({
			LinearColor{ light.color },
			light.intensity,
			light.radius,
			t.transform.location
		});
	});

	// Render camera
	QueueRenderCommand<DrawDirectionalLightCommand>(MoveTemp(directionals));
	QueueRenderCommand<DrawPointLightCommand>(MoveTemp(points));
}
