// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"

enum class EDoorState : u8 {
	Opened,
	Opening,
	Closed,
	Closing
};


class CDoor : public Component
{
	STRUCT(CDoor, Component)

	PROP(Name, keyId, DetailsEdit)
	Name keyId;

	PROP(u8, state, DetailsEdit)
	u8 state = u8(EDoorState::Closed);

	// Closed location assigned at BeginPlay
	PROP(v3, closedLocation)
	v3 closedLocation;

	PROP(v3, openedOffset, DetailsEdit)
	v3 openedOffset{ 0.f, 0.f, 1.5f };

	PROP(float, speed, DetailsEdit)
	float speed = 4.f;


	FORCEINLINE EDoorState GetState() const { return EDoorState(state); }
};
