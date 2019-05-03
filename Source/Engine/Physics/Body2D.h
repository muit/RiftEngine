// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Body.h>

#include "Core/Math/Vector.h"
#include "PhysicsTypes.h"


struct Body2D
{
private:

	b2Body* bodyPtr;

public:

	Body2D() : bodyPtr{ nullptr } {}
	Body2D(Body2D&& other) : bodyPtr{ other.bodyPtr } { other.bodyPtr = nullptr; }
	Body2D& operator=(Body2D&& other) {
		bodyPtr = other.bodyPtr;
		other.bodyPtr = nullptr;
	}

	void Initialize(b2World& world, const b2BodyDef& definition) {
		bodyPtr = world.CreateBody(&definition);
	}

	void SetMobility(EMobilityType mobility);

	bool IsValid() const { return bodyPtr != nullptr; }
	b2Body* GetRaw() const { return bodyPtr; }

	/** Call only after the body has been destroyed */
	void Reset() { bodyPtr = nullptr; }
};
