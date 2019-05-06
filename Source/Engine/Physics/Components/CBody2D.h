// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "../PhysicsTypes.h"
#include "../Body2D.h"


/** An entity will obtain physics when having a body.
 * The body specifies physical properties of the entity
 * like gravity, mobility type or fixed rotation.
 */
class CBody2D : public Component {
	STRUCT(CBody2D, Component)

public:

	// #TODO: Add support for reflected enums
	//EMobilityType mobility = EMobilityType::Movable;
	PROP(u8, mobility, DetailsEdit)
	u8 mobility = 2;

	PROP(bool, bAffectedByGravity, DetailsEdit);
	bool bAffectedByGravity = true;

	PROP(bool, bFixedRotation, DetailsEdit);
	bool bFixedRotation = false;

	/** Body used when this entity doesn't have a Body */
	Body2D body;


public:

	CBody2D() = default;

	void FillDefinition(b2BodyDef& def)
	{
		def.type = (b2BodyType)mobility;
		def.gravityScale = bAffectedByGravity ? 1.0f : 0.0f;
		def.fixedRotation = bFixedRotation;
	}

	FORCEINLINE bool IsStatic() const { return mobility == u8(EMobilityType::Static); }
};
