// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include <Box2D/Dynamics/b2World.h>

#include "Core/Engine.h"
#include "Core/MultiThreading.h"

#include "Gameplay/Components/CTransform.h"
#include "Components/CBoxCollider2D.h"
#include "Components/CCircleCollider2D.h"


class SPhysics2D : public System {
	CLASS(SPhysics2D, System)


	eastl::unique_ptr<b2World> world;

	// Cached
	class CPhysicsWorld* physicsWorld = nullptr;

	TaskFlow applyFlow;

public:

	SPhysics2D() : Super(), applyFlow{GEngine->Tasks().CreateFlow()} {}

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void EndPlay() override;

private:

	void Step(float deltaTime);

	//void UploadDataToPhysics();
	void ApplyPhysicsData();

	TaskLambda ApplyBodies();
	TaskLambda ApplyBoxes();
	TaskLambda ApplyCircles();

	void CreateAndUpdateBodies();
	void CreateFixtures();

private:

	EntityId FindBodyOwner(EntityId entity) const;
};
