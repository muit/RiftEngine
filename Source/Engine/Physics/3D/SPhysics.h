// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include <PxPhysicsAPI.h>
#include <PxFoundation.h>
#include <foundation/PxErrorCallback.h>
#include <extensions/PxDefaultCpuDispatcher.h>

#include "Core/Log.h"


class UserErrorCallback : public physx::PxErrorCallback
{
public:
	virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override
	{
		Log::Error(TX("PhysX error(%i): %s at %s:%i"), (i32)code, message, file, line);
	}
};


/**
 * System that runs on game to upload, step and apply physics simulation.
 * Upload: Creates bodies and fixtures or updates them if already existing
 * Step: Updates gravity and runs a simulation step
 * Apply: Applies simulation into entity transforms
 * @see CBody, CBoxCollider, CCircleCollider
 */
class SPhysics : public System {
	CLASS(SPhysics, System)

	physx::PxFoundation* foundation = nullptr;
	physx::PxPhysics* world = nullptr;
	physx::PxScene* scene = nullptr;
	physx::PxDefaultCpuDispatcher* cpuDispatcher = nullptr;

	UserErrorCallback pxErrorCallback {};

	// Cached
	class CPhysicsWorld* physicsWorld = nullptr;


public:

	SPhysics();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void EndPlay() override;
	virtual void BeforeDestroy() override;

	void CreateScene();

private:

	void Step(float deltaTime);
};
