// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "Box2D/Dynamics/b2World.h"

#include "Gameplay/Components/CTransform.h"
#include "Physics/Components/CBoxCollider2D.h"

#include "Fixture2D.h"
#include "Body2D.h"


class SPhysics2D : public System {
	CLASS(SPhysics2D, System)


	eastl::unique_ptr<b2World> world;

	// Cached
	class CPhysicsWorld* physicsWorld = nullptr;

public:

	SPhysics2D() : Super() {}

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void EndPlay() override;

private:

	void Step(float deltaTime);

	//void UploadDataToPhysics();
	void ApplyPhysicsData();

	void CreateAndUpdateBodies();
	void CreateFixtures();

private:

	EntityId FindBodyOwner(EntityId entity) const;
};
