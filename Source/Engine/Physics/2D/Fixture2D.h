// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include "PhysicsTypes2D.h"
#include "Body2D.h"


class b2Fixture;
struct b2FixtureDef;

/** Class that points to a Box2D fixture. Used by collider components */
struct Fixture2D
{
private:

	b2Fixture* fixturePtr = nullptr;


public:

	Fixture2D() : fixturePtr{ nullptr } {}
	Fixture2D(const Fixture2D& other) : fixturePtr{ nullptr } {}
	Fixture2D& operator=(const Fixture2D& other) { fixturePtr = nullptr; }
	Fixture2D(Fixture2D&& other) { eastl::swap(fixturePtr, other.fixturePtr); }
	Fixture2D& operator=(Fixture2D&& other) {
		eastl::swap(fixturePtr, other.fixturePtr);
		return *this;
	}

	void Initialize(Body2D& body, const b2FixtureDef& def) {
		fixturePtr = body.GetRaw()->CreateFixture(&def);
	}

	bool IsValid() const { return fixturePtr != nullptr; }
	b2Fixture* GetRaw() const { return fixturePtr; }

	/** Call only after the body has been destroyed */
	void Reset() { fixturePtr = nullptr; }


	template<typename ShapeType>
	v2 GetWorldLocation() {
		const v2 bodyPosition = fixturePtr->GetBody()->GetPosition();
		return bodyPosition + GetRelativeLocation<ShapeType>();
	}

	/** Get relative location in relation to its body based on a shape.
	 * @warning ShapeType must be the exact same type as the shape used for this fixture
	 */
	template<typename ShapeType>
	v2 GetRelativeLocation()
	{
		if constexpr (eastl::is_same<ShapeType, CircleShape>::value)
		{
			// Return the center of a circle shape
			return static_cast<CircleShape*>(fixturePtr->GetShape())->m_p;
		}
		else if constexpr (eastl::is_same<ShapeType, PolygonShape>::value)
		{
			// Return the center of a polygon shape
			return static_cast<PolygonShape*>(fixturePtr->GetShape())->m_centroid;
		}
		return v2::Zero();
	}
};
