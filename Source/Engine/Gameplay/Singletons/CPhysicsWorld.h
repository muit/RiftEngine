// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/CSingleton.h"

struct TriggerEvent
{
	EntityId triggerEntity;
	EntityId otherEntity;
};


class CPhysicsWorld : public CSingleton {
	STRUCT(CPhysicsWorld, CSingleton)

public:

	PROP(v3, gravity, DetailsEdit)
	v3 gravity {0.f, 0.f, -9.81f};


	TArray<TriggerEvent> triggerEvents;

	// Called every frame to clean previous events
	void ResetEvents()
	{
		triggerEvents.Empty();
	}
};
