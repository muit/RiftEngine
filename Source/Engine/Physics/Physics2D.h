// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Collision/Shapes/b2Shape.h>

#include "Core/Math/Vector.h"

class b2Body;
struct b2BodyDef;
class b2Fixture;
struct b2FixtureDef;
class Physics2D;


enum class EMobilityType : u8
{
	Static,
	Kinematic,
	Movable
};

enum class EShapeType : u8
{
	Box,
	Circle,
	Geometry
};

using Shape = b2Shape;
using PolygonShape = b2PolygonShape;


struct Fixture2D
{
	struct Parameters
	{
		const Shape* shape;

		void* containerPtr;

		// The friction coefficient, usually in the range [0,1].
		float friction = 0.2f;

		// The restitution (elasticity) usually in the range [0,1].
		float restitution = 0.f;

		// The density, usually in kg/m^2.
		float density = 0.f;

		// A sensor shape collects contact information but never generates a collision response.
		bool bIsTrigger = false;

		// Contact filtering data
		//CollisionFilter filter;

		FORCEINLINE b2FixtureDef ToB2Def() const;
	};

private:

	b2Fixture* fixturePtr;


public:

	Fixture2D() : fixturePtr{ nullptr } {}
	Fixture2D(Fixture2D&& other) : fixturePtr{ other.fixturePtr } { other.fixturePtr = nullptr; }
	Fixture2D& operator=(Fixture2D&& other) {
		fixturePtr = other.fixturePtr;
		other.fixturePtr = nullptr;
	}
	Fixture2D(b2Fixture* inFixture) : fixturePtr{ inFixture } {}


	bool IsValid() const { return fixturePtr != nullptr; }
	b2Fixture* GetRaw() const { return fixturePtr; }

};

struct Body2D
{
private:

	friend Physics2D;
	b2Body* bodyPtr;


public:

	Body2D() : bodyPtr{ nullptr } {}
	Body2D(Body2D&& other) : bodyPtr{ other.bodyPtr } { other.bodyPtr = nullptr; }
	Body2D& operator=(Body2D&& other) {
		bodyPtr = other.bodyPtr;
		other.bodyPtr = nullptr;
	}

private:

	Body2D(b2Body* inBody) : bodyPtr{ inBody } {}

public:

	void SetMobility(EMobilityType mobility);

	Fixture2D CreateFixture(const Fixture2D::Parameters& params);
	void DeleteFixture(Fixture2D& body);

	bool IsValid() const { return bodyPtr != nullptr; }
	b2Body* GetRaw() const { return bodyPtr; }

};


class Physics2D : public Object
{
	CLASS(Physics2D, Object)

	v2 gravity {0.0f, 10.f};

	b2World world;


public:

	Physics2D() : Super(), world{ ToB2Vec2(gravity) } {}

	void Initialize();
	void Tick(float deltaTime);
	virtual void BeforeDestroy() override;


	Body2D CreateBody(v2 position, EMobilityType mobility);
	void DeleteBody(Body2D& body);

private:

	b2Vec2 ToB2Vec2(v2 vector) { return { vector.x, vector.y }; }
	v2     ToV2(b2Vec2 vector) { return { vector.x, vector.y }; }
};
