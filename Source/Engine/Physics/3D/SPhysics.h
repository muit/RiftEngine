// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include <PxPhysicsAPI.h>
#include <PxFoundation.h>
#include <PxRigidBody.h>
#include <foundation/PxErrorCallback.h>
#include <extensions/PxDefaultCpuDispatcher.h>
#include <foundation/PxTransform.h>
#include <PxSimulationEventCallback.h>

#include "ECS/System.h"
#include "ECS/EntityId.h"
#include "Core/MultiThreading.h"
#include "Core/Log.h"

class CTransform;
class CBody;
class SPhysics;

class UserErrorCallback : public physx::PxErrorCallback
{
public:
	virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;
};


class SimulationCallback : public physx::PxSimulationEventCallback
{
	SPhysics* physics;

public:

	SimulationCallback(SPhysics* physics) : physics{ physics } {}

	// Implements PxSimulationEventCallback
	virtual void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) override {}
	virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override {}
	virtual void onSleep(physx::PxActor**, physx::PxU32) override {}
	virtual void onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32) override {}
	virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override {}

	virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
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

	float stepSize = 1.f / 60.f;

	physx::PxFoundation* foundation = nullptr;
	physx::PxPhysics* world = nullptr;
	physx::PxScene* scene = nullptr;
	//physx::PxDefaultCpuDispatcher* cpuDispatcher = nullptr;

	UserErrorCallback pxErrorCallback{};
	SimulationCallback simulationCallback;

	float deltaTimeIncrement = 0.0f;
	TaskFlow physicsMTFlow;

public:

	class CPhysicsWorld* physicsWorld = nullptr;


	SPhysics();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void EndPlay() override;
	virtual void BeforeDestroy() override;


private:

	void UploadBodies();
	void Step(float deltaTime);
	void DownloadBodies();

	void CreateScene();
	void CreateBody(EntityId entity, const CTransform& transform, CBody& body);
	void SetupBodyShapes(CBody& body);

public:

	static FORCEINLINE physx::PxVec3 ToPx(const v3& v) { return { v.x, v.y, v.z }; }
	static FORCEINLINE physx::PxQuat ToPx(const Quat& q) { return { q.x, q.y, q.z, q.w }; }
	static FORCEINLINE v3 FromPx(const physx::PxVec3& v) { return { v.x, v.y, v.z }; }
	static FORCEINLINE Quat FromPx(const physx::PxQuat& q) { return { q.x, q.y, q.z, q.w }; }

	static FORCEINLINE EntityId GetActorEntity(const physx::PxActor* actor) {
		// Threat void* as EntityId (only first 32 bits are used)
		const auto value = reinterpret_cast<size_t>(actor->userData);
		return static_cast<EntityId>(value);
	}

	static FORCEINLINE void SetActorEntity(physx::PxActor* actor, EntityId entity) {
		// Threat EntityId as void*
		const size_t value = entity;
		actor->userData = reinterpret_cast<void*>(value);
	}
};
