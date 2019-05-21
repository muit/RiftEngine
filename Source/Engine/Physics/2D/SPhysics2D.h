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

/**
 * System that runs on game to upload, step and apply physics simulation.
 * Upload: Creates bodies and fixtures or updates them if already existing
 * Step: Updates gravity and runs a simulation step
 * Apply: Applies simulation into entity transforms. This is done with multithreading
 * @see CBody2D, CBoxCollider2D, CCircleCollider2D
 */
class SPhysics2D : public System {
	CLASS(SPhysics2D, System)

	float stepSize = 1.f / 60.f;
	eastl::unique_ptr<b2World> world;

	// Cached
	class CPhysicsWorld* physicsWorld = nullptr;
	float deltaTimeIncrement = 0.0f;

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
	void DownloadBodies();

	TaskLambda ApplyBodies();
	TaskLambda ApplyBoxes();
	TaskLambda ApplyCircles();

	void CreateAndUploadBodies();
	void CreateFixtures();

private:

	EntityId FindBodyOwner(EntityId entity) const;
};
