// Copyright 2015-2019 Piperift - All rights reserved

#include "SPhysics2D.h"
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include "World.h"
#include "Tools/Profiler.h"

#include "PhysicsTypes.h"
#include "Components/CBody2D.h"
#include "Gameplay/Singletons/CPhysicsWorld.h"


void SPhysics2D::BeginPlay()
{
	Super::BeginPlay();

	physicsWorld = ECS()->FindSingleton<CPhysicsWorld>();

	world = eastl::make_unique<b2World>(b2Vec2{ physicsWorld->gravity });
}

void SPhysics2D::Tick(float deltaTime)
{
	ScopedStackGameZone();
	Super::Tick(deltaTime);

	// Bodies & Fixtures
	{
		CreateAndUpdateBodies();
		CreateFixtures();
	}

	Step(deltaTime);

	ApplyPhysicsData();

	Log::Message("Physics Dump");
	world->Dump();
}

void SPhysics2D::EndPlay()
{
	world.reset();
	Super::EndPlay();
}

void SPhysics2D::ApplyPhysicsData()
{
	const auto ecs = ECS();

	// Update bodies from physics
	auto bodyView = ecs->View<CTransform, CBody2D>();
	for (auto entity : bodyView)
	{
		CTransform& transform = bodyView.get<CTransform>(entity);
		CBody2D&    bodyComp  = bodyView.get<CBody2D>(entity);

		if (bodyComp.body.IsValid())
		{
			const v2 position = bodyComp.body.GetLocation();
			transform.SetWLocation(position.xz());
		}
	}

	// Update fixtures from physics
	auto boxView = ecs->View<CTransform, CBoxCollider2D>();
	for (auto entity : boxView)
	{
		// Bodies are already updated, ignore entities with them
		if (!ecs->Has<CBody2D>(entity))
		{
			CTransform& transform    = boxView.get<CTransform>(entity);
			CBoxCollider2D& collider = boxView.get<CBoxCollider2D>(entity);

			const v2 position = collider.fixture.GetWorldLocation<PolygonShape>();
			transform.SetWLocation(position.xz());
		}
	}
}

void SPhysics2D::CreateAndUpdateBodies()
{
	auto view = ECS()->View<CTransform, CBody2D>();
	for (auto entity : view)
	{
		CTransform& transform = view.get<CTransform>(entity);
		CBody2D& bodyComp     = view.get<CBody2D>(entity);

		Body2D& body = bodyComp.body;
		if (!body.IsValid())
		{
			b2BodyDef bodyDef;
			const v3 location = transform.GetWLocation();
			bodyDef.position = location.xz();
			bodyDef.angle = transform.GetWRotation().y;

			bodyComp.FillDefinition(bodyDef);
			body.Initialize(*world, bodyDef);
		}
		else
		{
			const v2 location = transform.GetWLocation().xz();
			const float angle = transform.GetWRotation().y;
			const v2 currLocation = body.GetLocation();
			const float currAngle = body.GetAngle();

			// If angle or position changed, update it
			if (!Math::NearlyEqual(angle, currAngle) || location.DistanceSqrt(currLocation) > Math::SMALL_NUMBER)
			{
				body.SetTransform(location, angle);
			}
		}
	}
}

void SPhysics2D::CreateFixtures()
{
	const auto ecs = ECS();
	auto view = ecs->View<CTransform, CBoxCollider2D>();

	/** For each Collider component, find a body and registry it as a fixture */
	view.each([this, ecs](EntityId e, const CTransform& tComp, CBoxCollider2D& collider)
	{
		Fixture2D& fixture = collider.fixture;
		if (!fixture.IsValid())
		{
			EntityId bodyEntity = FindBodyOwner(e);

			// No body? No fixture
			if (bodyEntity == NoEntity)
				return;

			CBody2D & body = ecs->Get<CBody2D>(bodyEntity);

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
	// Update gravity if changed
	if (physicsWorld)
	{
		world->SetGravity(physicsWorld->gravity);
	}

	const int32 velocityIterations = 6;
	const int32 positionIterations = 2;
	world->Step(deltaTime, velocityIterations, positionIterations);
}
