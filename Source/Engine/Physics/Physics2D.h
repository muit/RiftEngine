// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include <Box2D/Dynamics/b2World.h>

#include "Core/Math/Vector.h"

class b2Body;
class Physics2D;


enum class EMobilityType : u8
{
	Static,
	Kinematic,
	Movable
};


struct Body2D
{
	friend Physics2D;

	b2Body* bodyPtr;


	Body2D() : bodyPtr{ nullptr } {}
	Body2D(Body2D&& other) : bodyPtr{ other.bodyPtr } { other.bodyPtr = nullptr; }

	bool IsValid() const { return bodyPtr != nullptr; }

private:

	Body2D(b2Body* inBody) : bodyPtr{ inBody } {}
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

	b2Vec2 ToB2Vec2(v2 vector) { return { vector.x(), vector.y() }; }
	v2     ToV2(b2Vec2 vector) { return { vector.x, vector.y }; }
};
