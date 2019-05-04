// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "../PhysicsTypes.h"
#include "../Body2D.h"


class CBody2D : public Component {
	STRUCT(CBody2D, Component)

public:

	// #TODO: Add support for reflected enums
	//EMobilityType mobility = EMobilityType::Movable;
	PROP(u8, mobility)
	u8 mobility = 2;

	PROP(bool, bAffectedByGravity, DetailsEdit);
	bool bAffectedByGravity = true;

	PROP(bool, bFixedRotation, DetailsEdit);
	bool bFixedRotation = false;

	/** Body used when this entity doesn't have a Body */
	Body2D body;


public:

	CBody2D() = default;
};
