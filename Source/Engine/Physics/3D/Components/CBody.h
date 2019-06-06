// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "PxRigidActor.h"
#include "PxRigidDynamic.h"

#include "../../PhysicsTypes.h"
#include "../SPhysics.h"
#include "Tools/Profiler.h"


/** An entity will obtain physics when having a body.
 * The body specifies physical properties of the entity
 * like gravity, mobility type or fixed rotation.
 */
class CBody : public Component {
	STRUCT(CBody, Component)


	physx::PxRigidActor* rigidBody = nullptr;

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


	bool IsInitialized() const { return rigidBody != nullptr; }

	void SetLinearVelocity(const v3& velocity)
	{
		if (!IsStatic())
		{
			AsDynamic()->setLinearVelocity(SPhysics::ToPx(velocity));
		}
	}

	void AddAcceleration(const v3& accel)
	{
		if (!IsStatic())
		{
			AsDynamic()->addForce(SPhysics::ToPx(accel), physx::PxForceMode::eACCELERATION);
		}
	}

	void AddForce(const v3& force, bool bAsImpulse = false)
	{
		if (!IsStatic())
		{
			AsDynamic()->addForce(SPhysics::ToPx(force), bAsImpulse? physx::PxForceMode::eIMPULSE : physx::PxForceMode::eFORCE);
		}
	}

	void ClearForces()
	{
		if (!IsStatic())
		{
			AsDynamic()->clearForce(physx::PxForceMode::eFORCE);
			AsDynamic()->clearForce(physx::PxForceMode::eIMPULSE);
		}
	}

	void SetTransform(const v3& location, const Quat& rotation) const
	{
		switch (EMobilityType(mobility))
		{
		case EMobilityType::Movable:
			AsDynamic()->setGlobalPose({ SPhysics::ToPx(location), SPhysics::ToPx(rotation) });
			break;
		case EMobilityType::Kinematic:
			AsDynamic()->setKinematicTarget({ SPhysics::ToPx(location), SPhysics::ToPx(rotation) });
			break;
		default:
			Ensure(false, "Only Movable or Kinematic bodies can SetTransform");
		}
	}

	FORCEINLINE bool IsStatic() const { return mobility == u8(EMobilityType::Static); }
	FORCEINLINE bool IsKinematic() const { return mobility == u8(EMobilityType::Kinematic); }

private:

	/** This assumes the body is dynamic. Otherwise its dangerous to call */
	FORCEINLINE physx::PxRigidDynamic* AsDynamic() const
	{
		return static_cast<physx::PxRigidDynamic*>(rigidBody);
	}
};
