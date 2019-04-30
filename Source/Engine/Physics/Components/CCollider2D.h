// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "Physics/Physics2D.h"


enum class EColliderFlag : u8 {
	Dirty           = 1 << 0,
	PendingCreation = 1 << 1,
	PendingDeletion = 1 << 2
};


class CCollider2D : public Component {
	STRUCT(CCollider2D, Component)

public:
	// #TODO: Add support for reflected enums
	EMobilityType mobility = EMobilityType::Movable;

	PROP(float, density, DetailsEdit);
	float density = 1;

	PROP(bool, bTrigger, DetailsEdit);
	bool bTrigger = false;

	PROP(bool, bAffectedByGravity, DetailsEdit);
	bool bAffectedByGravity = true;

	PROP(bool, bFixedRotation, DetailsEdit);
	bool bFixedRotation = false;

	u8 flags = u8(EColliderFlag::PendingCreation);

	/** Body used when this entity doesn't have a Rigidbody */
	Body2D staticBody;
	Fixture2D fixture;


public:

	CCollider2D() = default;
	CCollider2D(CCollider2D&&) = default;
	CCollider2D& operator=(CCollider2D&&) = default;
	CCollider2D(const CCollider2D& other) { CopyFrom(other); }
	CCollider2D& operator=(const CCollider2D& other) { CopyFrom(other); }

	virtual void SetupProperties()
	{
	}
	virtual void UpdateProperties()
	{
	}


	bool IsDirty() const { return (flags & u8(EColliderFlag::Dirty)) >= 0; }

protected:

	void MarkDirty()
	{
		flags |= u8(EColliderFlag::Dirty);
	}

private:

	void CopyFrom(const CCollider2D& other)
	{
		density            = other.density;
		bAffectedByGravity = other.bAffectedByGravity;
		bTrigger           = other.bTrigger;
		bFixedRotation     = other.bFixedRotation;
		flags = other.flags | u8(EColliderFlag::PendingCreation) | u8(EColliderFlag::Dirty);
		staticBody = {};
	}
};
