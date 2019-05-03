// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include "PhysicsTypes.h"
#include "Body2D.h"


class b2Fixture;
struct b2FixtureDef;

struct Fixture2D
{
private:

	b2Fixture* fixturePtr;


public:

	Fixture2D() : fixturePtr{ nullptr } {}
	Fixture2D(const Fixture2D& other) : fixturePtr{ nullptr } {}
	Fixture2D& operator=(const Fixture2D& other) { fixturePtr = nullptr; }
	Fixture2D(Fixture2D&& other) : fixturePtr{ other.fixturePtr } { other.fixturePtr = nullptr; }
	Fixture2D& operator=(Fixture2D&& other) {
		fixturePtr = other.fixturePtr;
		other.fixturePtr = nullptr;
	}

	void Initialize(Body2D& body, const b2FixtureDef& def) {
		fixturePtr = body.GetRaw()->CreateFixture(&def);
	}

	bool IsValid() const { return fixturePtr != nullptr; }
	b2Fixture* GetRaw() const { return fixturePtr; }

	/** Call only after the body has been destroyed */
	void Reset() { fixturePtr = nullptr; }
};
