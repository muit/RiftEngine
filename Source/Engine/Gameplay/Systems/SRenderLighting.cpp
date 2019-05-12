// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderLighting.h"
#include "Core/Engine.h"
#include "Tools/Profiler.h"

#include "../Components/CDirectionalLight.h"
#include "../Components/CPointLight.h"
#include "Rendering/Commands/LightingCommands.h"


void SRenderLighting::Tick(float deltaTime)
{
	ScopedStackGameZone();
	Super::Tick(deltaTime);

	TArray<DirectionalLightData> directionals {};
	TArray<PointLightData> points {};

	// Gather directionals data
	auto dirView = ECS()->View<CTransform, CDirectionalLight>();
	directionals.Reserve((i32)dirView.size());
	dirView.each([deltaTime, &directionals](EntityId e, CTransform& t, CDirectionalLight& light)
	{
		directionals.Add({
			LinearColor{ light.color },
			light.intensity,
			t.transform.rotation
		});
	});

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
