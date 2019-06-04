// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "PxRigidActor.h"
#include "PxRigidDynamic.h"

#include "../../PhysicsTypes.h"
#include "../SPhysics.h"


/** An entity will obtain physics when having a body.
 * The body specifies physical properties of the entity
 * like gravity, mobility type or fixed rotation.
 */
class CBody : public Component {
	STRUCT(CBody, Component)


	physx::PxRigidActor* rigidBody;

public:

	// #TODO: Add support for reflected enums
	//EMobilityType mobility = EMobilityType::Movable;
	PROP(u8, mobility, DetailsEdit)
	u8 mobility = 2;

	PROP(bool, bAffectedByGravity, DetailsEdit);
	bool bAffectedByGravity = true;

	// #TODO: Replace with TArray of shapes
	PROP(bool, bHasBoxShape, DetailsEdit);
	bool bHasBoxShape = true;

	PROP(v3, boxExtent, DetailsEdit);
	v3 boxExtent{ 0.5f };

	PROP(bool, bHasSphereShape, DetailsEdit);
	bool bHasSphereShape = false;

	PROP(float, radius, DetailsEdit);
	float radius{ 0.5f };

	PROP(bool, bIsTrigger, DetailsEdit);
	bool bIsTrigger = false;


	CBody() = default;

	CBody(CBody&& other) : Super(other) {
		eastl::swap(rigidBody, other.rigidBody);
	}
	CBody& operator=(CBody&& other) {
		eastl::swap(rigidBody, other.rigidBody);
		return *this;
	}

	bool IsInitialized() const { return rigidBody != nullptr; }

	void SetLinearVelocity(const v3& velocity)
	{
		if (!IsStatic())
		{
			AsDynamic()->setLinearVelocity(SPhysics::ToPx(velocity));
		}
	}


	FORCEINLINE bool IsStatic() const { return mobility == u8(EMobilityType::Static); }

private:

	/** This assumes the body is dynamic. Otherwise its dangerous to call */
	FORCEINLINE physx::PxRigidDynamic* AsDynamic() const
	{
		return static_cast<physx::PxRigidDynamic*>(rigidBody);
	}
};
