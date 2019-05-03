// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "Box2D/Dynamics/b2World.h"

#include "Gameplay/Components/CTransform.h"
#include "Physics/Components/CBoxCollider2D.h"

#include "Fixture2D.h"
#include "Body2D.h"


using BoxView = entt::View<EntityId, CTransform, CBoxCollider2D>;


class SPhysics2D : public System {
	CLASS(SPhysics2D, System)


	b2World world;


public:

	SPhysics2D() : Super(), world{ToB2Vec2({0.0f, 10.f})} {}

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

private:

	void Step(float deltaTime);

	void UploadDataToPhysics();
	void ApplyPhysicsData();

	void CreateBodies();
	void CreateFixtures();

private:

	EntityId FindBodyOwner(EntityId entity) const;

	static b2Vec2 ToB2Vec2(v2 vector) { return { vector.x, vector.y }; }
	static v2     ToV2(b2Vec2 vector) { return { vector.x, vector.y }; }
};
