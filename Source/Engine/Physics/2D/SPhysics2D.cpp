// Copyright 2015-2019 Piperift - All rights reserved

#include "SPhysics2D.h"
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include "World.h"
#include "Core/MultiThreading.h"
#include "Tools/Profiler.h"

#include "PhysicsTypes2D.h"
#include "Components/CBody2D.h"
#include "Gameplay/Singletons/CPhysicsWorld.h"

#include "Fixture2D.h"
#include "Body2D.h"


void SPhysics2D::BeginPlay()
{
	Super::BeginPlay();

	physicsWorld = ECS()->FindSingleton<CPhysicsWorld>();

	world = eastl::make_unique<b2World>(b2Vec2{ physicsWorld->gravity.xz() });
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
}

void SPhysics2D::EndPlay()
{
	world.reset();
	Super::EndPlay();
}

void SPhysics2D::ApplyPhysicsData()
{
	ScopedGameZone("Apply physics data");
	// No bodies, boxes or circles update ever the same entity, so no collision
	applyFlow.emplace(
		ApplyBodies(),
		ApplyBoxes(),
		ApplyCircles()
	);
	applyFlow.wait_for_all();
}

TaskLambda SPhysics2D::ApplyBodies()
{
	// Update bodies from physics
	auto ecs = ECS();
	return [ecs]() {
		ScopedGameZone("Apply Bodies");

		auto bodyView = ecs->View<CTransform, CBody2D>();
		for (auto entity : bodyView)
		{
			CTransform& transform = bodyView.get<CTransform>(entity);
			CBody2D& bodyComp = bodyView.get<CBody2D>(entity);

			if (bodyComp.body.IsValid())
			{
				const v2 position = bodyComp.body.GetLocation();
				transform.SetWLocation(position.xz());

				// Update angle
				//transform.SetWRotation(Quat::FromRotator({ 0.f, bodyComp.body.GetAngle(), 0.f }));
			}
		}
	};
}

TaskLambda SPhysics2D::ApplyBoxes()
{
	// Update box fixtures from physics
	auto ecs = ECS();
	return [ecs]() {
		ScopedGameZone("Apply Boxes");

		auto boxView = ecs->View<CTransform, CBoxCollider2D>();
		for (auto entity : boxView)
		{
			// Bodies are already updated, ignore entities with them
			if (!ecs->Has<CBody2D>(entity))
			{
				CBoxCollider2D& collider = boxView.get<CBoxCollider2D>(entity);
				if (collider.fixture.IsValid())
				{
					CTransform& transform = boxView.get<CTransform>(entity);

					const v2 position = collider.fixture.GetWorldLocation<PolygonShape>();
					transform.SetWLocation(position.xz());
				}
			}
		}
	};
}

TaskLambda SPhysics2D::ApplyCircles()
{
	// Upload circle fixtures
	auto ecs = ECS();
	return [ecs]() {
		ScopedGameZone("Apply Circles");

		auto circleView = ecs->View<CTransform, CCircleCollider2D>();
		for (auto entity : circleView)
		{
			// If has a body or another collider, ignore
			if (!ecs->Has<CBody2D>(entity) && !ecs->Has<CBoxCollider2D>(entity))
			{
				CCircleCollider2D& collider = circleView.get<CCircleCollider2D>(entity);
				if (collider.fixture.IsValid())
				{
					CTransform& transform = circleView.get<CTransform>(entity);

					const v2 position = collider.fixture.GetWorldLocation<CircleShape>();
					transform.SetWLocation(position.xz());
				}
			}
		}
	};
}

void SPhysics2D::CreateAndUpdateBodies()
{
	ScopedGameZone("Create & Update Bodies");

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
			bodyDef.angle = transform.GetWRotation().ToRotator().y * Math::DEGTORAD;

			bodyComp.FillDefinition(bodyDef);
			body.Initialize(*world, bodyDef);
		}
		else if(!bodyComp.IsStatic())
		{
			const v2 location = transform.GetWLocation().xz();
			const float angle = transform.GetWRotation().ToRotator().y * Math::DEGTORAD;
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
	ScopedGameZone("Create Fixtures");

	const auto ecs = ECS();
	auto boxView = ecs->View<CTransform, CBoxCollider2D>();

	/** For each Box Collider, find a body and registry it as a fixture */
	boxView.each([this, ecs](EntityId e, const CTransform& transform, CBoxCollider2D& collider)
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
			collider.FillDefinition(def, &shape, transform.GetWRotation().ToRotator().y);
			def.shape = &shape;
			fixture.Initialize(body.body, def);
		}
	});

	/** For each Circle Collider, find a body and registry it as a fixture */
	auto circleView = ecs->View<CTransform, CCircleCollider2D>();
	circleView.each([this, ecs](EntityId e, const CTransform& tComp, CCircleCollider2D& collider)
	{
		Fixture2D& fixture = collider.fixture;
		if (!fixture.IsValid())
		{
			EntityId bodyEntity = FindBodyOwner(e);

			// No body? No fixture
			if (bodyEntity == NoEntity)
				return;

			CBody2D & body = ecs->Get<CBody2D>(bodyEntity);

			CircleShape shape;
			b2FixtureDef def;
			collider.FillDefinition(def, &shape, 0.f);
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
	ScopedGameZone("Step");
	// Update gravity if changed
	if (physicsWorld)
	{
		world->SetGravity(physicsWorld->gravity.xz());
	}

	const int32 velocityIterations = 6;
	const int32 positionIterations = 2;
	world->Step(deltaTime, velocityIterations, positionIterations);
}
