// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Body.h>

#include "Core/Math/Vector.h"
#include "PhysicsTypes2D.h"


/** Class that points to a Box2D body. Used by body components */
struct Body2D
{
private:

	b2Body* bodyPtr = nullptr;

public:

	Body2D() {}
	Body2D(Body2D&& other) { eastl::swap(bodyPtr, other.bodyPtr); }
	Body2D& operator=(Body2D&& other) {
		eastl::swap(bodyPtr, other.bodyPtr);
		return *this;
	}

	void Initialize(b2World& world, const b2BodyDef& definition) {
		bodyPtr = world.CreateBody(&definition);
	}

	void SetMobility(EMobilityType mobility);

	FORCEINLINE v2 GetLocation() { return bodyPtr->GetPosition(); }
	FORCEINLINE float GetAngle() { return bodyPtr->GetAngle(); }

	void SetTransform(v2 location, float angle) {
		bodyPtr->SetTransform(location, angle);
	}

	void SetLinearVelocity(v2 velocity) const {
		bodyPtr->SetLinearVelocity(velocity);
	}
	void SetAngularVelocity(float velocity) const {
		bodyPtr->SetAngularVelocity(velocity);
	}
	void ApplyLinearImpulse(v2 impulse, bool bWake = true) const {
		bodyPtr->ApplyLinearImpulseToCenter(impulse, bWake);
	}
	void ApplyLinearImpulse(v2 impulse, v2 point, bool bWake = true) const {
		bodyPtr->ApplyLinearImpulse(impulse, point, bWake);
	}
	void ApplyForce(v2 impulse, bool bWake = true) const {
		bodyPtr->ApplyForceToCenter(impulse, bWake);
	}
	void ApplyForce(v2 impulse, v2 point, bool bWake = true) const {
		bodyPtr->ApplyForce(impulse, point, bWake);
	}
	void ApplyTorque(float torque, bool bWake = true) const {
		bodyPtr->ApplyTorque(torque, bWake);
	}

	bool IsValid() const { return bodyPtr != nullptr; }
	b2Body* GetRaw() const { return bodyPtr; }

	/** Call only after the body has been destroyed */
	void Reset() { bodyPtr = nullptr; }
};
