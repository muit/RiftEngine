// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"


/**
 * System that runs on game to upload, step and apply physics simulation.
 * Upload: Creates bodies and fixtures or updates them if already existing
 * Step: Updates gravity and runs a simulation step
 * Apply: Applies simulation into entity transforms
 * @see CBody, CBoxCollider, CCircleCollider
 */
class SPhysics : public System {
	CLASS(SPhysics, System)


	class PxFoundation* fundation;

	// Cached
	class CPhysicsWorld* physicsWorld = nullptr;


protected:

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void EndPlay() override;

private:

	void Step(float deltaTime);
};
