// Copyright 2015-2019 Piperift - All rights reserved

#include "SPhysics2D.h"
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include "World.h"
#include "Tools/Profiler.h"

#include "PhysicsTypes.h"
#include "Components/CBody2D.h"


void SPhysics2D::BeginPlay()
{
	Super::BeginPlay();
	world = { ToB2Vec2({0.0f, 10.f}) };
}

void SPhysics2D::Tick(float deltaTime)
{
	ScopedStackGameZone();
	Super::Tick(deltaTime);

	// Body & Fixture creation
	{
		CreateBodies();
		CreateFixtures();
	}

	//BoxView view = ECS()->View<CTransform, CBoxCollider2D>();


	Step(deltaTime);

	ApplyPhysicsData();
}

/*void SPhysics2D::UploadDataToPhysics(BoxView& view)
{
	view.each([](EntityId e, CTransform& t, CBoxCollider2D& collider)
	{
		// Update transforms
		if (collider.IsDirty())
		{
			// Update body

		}
	});
}*/

void SPhysics2D::ApplyPhysicsData()
{
	const auto ecs = ECS();
	auto view = ecs->View<CTransform, CBoxCollider2D>();

	for (auto entity : view)
	{
		//CTransform& transform    = view.get<CTransform>(entity);
		CBoxCollider2D& collider = view.get<CBoxCollider2D>(entity);

		EntityId bodyEntity = FindBodyOwner(entity);

		// No body? No fixture
		if (bodyEntity == NoEntity)
			return;

		CBody2D& body = ecs->Get<CBody2D>(bodyEntity);

		Fixture2D & fixture = collider.fixture;
		assert(fixture.IsValid());

		// Fixture on the same entity, position is the same
		if (bodyEntity == entity)
		{

		}
		else
		{
			// #TODO: Update relative transform
		}
	}
}

void SPhysics2D::CreateBodies()
{
	auto view = ECS()->View<CTransform, CBody2D>();

	b2World* worldPtr = &world;
	view.each([worldPtr](EntityId e, const CTransform& tComp, CBody2D& bodyComp)
	{
		Body2D& body = bodyComp.body;
		if (!body.IsValid())
		{
			b2BodyDef bodyDef;
			bodyDef.position = ToB2Vec2(tComp.GetWLocation());
			bodyDef.type = (b2BodyType)bodyComp.mobility;
			body.Initialize(*worldPtr, bodyDef);
		}
	});
}

void SPhysics2D::CreateFixtures()
{
	const auto ecs = ECS();

	auto view = ecs->View<CTransform, CBoxCollider2D>();

	/** For each Collider component, find a body and registry it as a fixture */
	view.each([this, ecs](EntityId e, const CTransform& tComp, CBoxCollider2D& collider)
	{
		EntityId bodyEntity = FindBodyOwner(e);

		// No body? No fixture
		if (bodyEntity == NoEntity)
			return;

		CBody2D& body = ecs->Get<CBody2D>(bodyEntity);

		Fixture2D& fixture = collider.fixture;
		if (!fixture.IsValid())
		{
			PolygonShape shape;
			b2FixtureDef def;
			collider.FillDefinition(def, &shape);
			def.shape = &shape;
			fixture.Initialize(body.body, def);
		}
	});
}

EntityId SPhysics2D::FindBodyOwner(EntityId entity) const
{
	const auto ecs = ECS();

	while (entity != NoEntity)
	{
		if (ecs->Has<CBody2D>(entity))
		{
			// Return first entity with a body
			return entity;
		}
		else if (ecs->Has<CTransform>(entity))
		{
			entity = ecs->Get<CTransform>(entity).parent;
		}
		else
		{
			return NoEntity;
		}
	}
	return NoEntity;
}

void SPhysics2D::Step(float deltaTime)
{
	const int32 velocityIterations = 6;
	const int32 positionIterations = 2;
	world.Step(deltaTime, velocityIterations, positionIterations);
}
