// Copyright 2015-2019 Piperift - All rights reserved

#include "SPhysics.h"
#include "World.h"
#include "Tools/Profiler.h"


void SPhysics::Tick(float deltaTime)
{
	ScopedStackGameZone();
	Super::Tick(deltaTime);

	auto physics = GetWorld()->GetPhysics();

	BoxView view = ECS()->View<CTransform, CBoxCollider2D>();



	// Update physics step
	physics->Tick(deltaTime);

	ApplyPhysicsData(view);
}

void SPhysics::UploadDataToPhysics(BoxView& view)
{
	view.each([](EntityId e, CTransform& t, CBoxCollider2D& collider)
	{
		// Update transforms
		if (collider.IsDirty())
		{
			// Update body

		}
	});
}

void SPhysics::ApplyPhysicsData(BoxView& view)
{
	view.each([](EntityId e, CTransform& t, CBoxCollider2D& collider)
	{
		// Apply transformations
		//t.GetWorldTransform()
	});
}
