// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include <Box2D/Dynamics/b2Fixture.h>

#include "ECS/Component.h"
#include "Physics/2D/Fixture2D.h"


enum class EColliderFlag : u8 {
	Dirty = 1 << 0 // Used to detect changes and later update a collider's physics
};


/**
 * Specifies a shape with physical properties crelated to a body
 * Always requires a body on the same entity or on a parent entity.
 * A Collider can't change its relative location in runtime
 */
class CCollider2D : public Component {
	STRUCT(CCollider2D, Component)

public:

	// The density, usually in kg/m^2.
	PROP(float, density, DetailsEdit);
	float density = 1;

	// The friction coefficient, usually in the range [0,1].
	PROP(float, friction, DetailsEdit);
	float friction = 0.2f;

	// The restitution (elasticity) usually in the range [0,1].
	PROP(float, restitution, DetailsEdit);
	float restitution = 0.f;

	PROP(bool, bIsTrigger, DetailsEdit);
	bool bIsTrigger = false;

	PROP(bool, bAffectedByGravity, DetailsEdit);
	bool bAffectedByGravity = true;

	PROP(bool, bFixedRotation, DetailsEdit);
	bool bFixedRotation = false;

	PROP(u8, flags, Transient)
	u8 flags = 0u;

	Fixture2D fixture;


public:

	CCollider2D() = default;

	bool IsDirty() const { return (flags & u8(EColliderFlag::Dirty)) >= 0; }

protected:

	void MarkDirty()
	{
		flags |= u8(EColliderFlag::Dirty);
	}

public:

	virtual void FillDefinition(b2FixtureDef& def, Shape* shape, float angle) const
	{
		def.density = density;
		def.friction = friction;
		def.restitution = restitution;
		def.isSensor = bIsTrigger;
	}
};
