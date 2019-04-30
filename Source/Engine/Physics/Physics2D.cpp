// Copyright 2015-2019 Piperift - All rights reserved

#include "Physics2D.h"
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>




b2FixtureDef Fixture2D::Parameters::ToB2Def() const
{
	b2FixtureDef def;
	def.shape       = shape;
	def.friction    = friction;
	def.restitution = restitution;
	def.density     = density;
	def.isSensor    = bIsTrigger;
	return def;
}

void Body2D::SetMobility(EMobilityType mobility)
{
	assert(bodyPtr);
	bodyPtr->SetType(b2BodyType(mobility));
}

Fixture2D Body2D::CreateFixture(const Fixture2D::Parameters& params)
{
	if (IsValid())
	{
		b2FixtureDef def = params.ToB2Def();
		return { bodyPtr->CreateFixture(&def) };
	}
	return {};
}

void Body2D::DeleteFixture(Fixture2D& body)
{

}

void Physics2D::Initialize()
{
}

void Physics2D::Tick(float deltaTime)
{
	const int32 velocityIterations = 6;
	const int32 positionIterations = 2;
	world.Step(deltaTime, velocityIterations, positionIterations);
}

void Physics2D::BeforeDestroy()
{
	Super::BeforeDestroy();
}

Body2D Physics2D::CreateBody(v2 position, EMobilityType mobility)
{
	b2BodyDef bodyDef;
	bodyDef.position = ToB2Vec2(position);
	bodyDef.type = (b2BodyType)mobility;
	return { world.CreateBody(&bodyDef) };
}

void Physics2D::DeleteBody(Body2D& body)
{
	world.DestroyBody(body.bodyPtr);
	body.bodyPtr = nullptr;
}
