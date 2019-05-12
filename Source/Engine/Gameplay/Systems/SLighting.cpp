// Copyright 2015-2019 Piperift - All rights reserved

#include "SLighting.h"
#include "Tools/Profiler.h"

#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CDirectionalLight.h"


void SLighting::Tick(float deltaTime)
{
	ScopedStackGameZone();
	Super::Tick(deltaTime);

	// Gather directionals data
	auto dirView = ECS()->View<CTransform, CDirectionalLight>();
	dirView.each([deltaTime](EntityId e, CTransform& t, CDirectionalLight& light)
	{
		// Rotate the light
		Rotator r = t.transform.GetRotation();
		r.x += 10.f * deltaTime;
		r.z += 1.f * deltaTime;
		t.transform.SetRotation(r);
	});
}
