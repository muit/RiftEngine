// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Body.h>

#include "Core/Math/Vector.h"
#include "PhysicsTypes.h"


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

	bool IsValid() const { return bodyPtr != nullptr; }
	b2Body* GetRaw() const { return bodyPtr; }

	/** Call only after the body has been destroyed */
	void Reset() { bodyPtr = nullptr; }
};
