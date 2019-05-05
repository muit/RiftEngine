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
		Shape* shape = fixturePtr->GetShape();

		if constexpr (eastl::is_same<ShapeType, CircleShape>::value)
		{
			// Return the center of a circle shape
			return static_cast<CircleShape*>(fixturePtr->GetShape())->m_p;
		}
		else if (eastl::is_same<ShapeType, PolygonShape>::value)
		{
			// Return the center of a polygon shape
			return static_cast<PolygonShape*>(fixturePtr->GetShape())->m_centroid;
		}
		return v2::Zero();
	}
};
