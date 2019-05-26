// Copyright 2015-2019 Piperift - All rights reserved

#include "SPhysics.h"
#include <common/PxTolerancesScale.h>
#include <extensions/PxRigidActorExt.h>
#include <foundation/PxFlags.h>
#include <geometry/PxBoxGeometry.h>
#include <geometry/PxSphereGeometry.h>
#include <PxSceneDesc.h>
#include <PxFiltering.h>
#include <PxMaterial.h>

#include "Core/Engine.h"
#include "World.h"
#include "Tools/Profiler.h"

#include "Gameplay/Components/CTransform.h"
#include "Components/CBody.h"

#include "../PhysicsTypes.h"
#include "Gameplay/Singletons/CPhysicsWorld.h"


using namespace physx;


void UserErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
	Log::Error(TX("PhysX error(%i): %s at %s:%i"), (i32)code, message, file, line);
}

void SimulationCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	if (!physics->physicsWorld)
		return;

	for (PxU32 i = 0; i < count; i++)
	{
		PxTriggerPair& pair = pairs[i];
		// Ignore pairs when shapes have been deleted
		if (pair.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;

		physics->physicsWorld->triggerEvents.Add(TriggerEvent{
			SPhysics::GetActorEntity(pair.triggerActor),
			SPhysics::GetActorEntity(pair.otherActor)
		});
	}
}


PxFilterFlags CollisionFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// let triggers through
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlag::eDEFAULT;
}


SPhysics::SPhysics()
	: Super()
	, simulationCallback{this}
	, physicsMTFlow{ GEngine->Tasks().CreateFlow() }
{
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *Memory::GetPhysicsAllocator(), pxErrorCallback);
	if (!foundation)
	{
		Log::Error("PhysX error: PxCreateFoundation failed");
		return;
	}

	physx::PxTolerancesScale scale;
	world = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, {});
	if (!world)
	{
		Log::Error("PhysX error: PxCreatePhysics failed");
		return;
	}
}

void SPhysics::BeginPlay()
{
	Super::BeginPlay();

	physicsWorld = ECS()->FindSingleton<CPhysicsWorld>();
	CreateScene();
}

void SPhysics::Tick(float deltaTime)
{
	ScopedStackGameZone();
	Super::Tick(deltaTime);

	if (physicsWorld)
	{
		physicsWorld->ResetEvents();
	}

	// Simulate at a fixed rate
	deltaTimeIncrement += deltaTime;
	if (deltaTimeIncrement >= stepSize)
	{
		deltaTimeIncrement -= stepSize;

		UploadBodies();

		Step(stepSize);

		DownloadBodies();
	}
}

void SPhysics::EndPlay()
{
	scene->release();
	scene = nullptr;
	Super::EndPlay();
}

void SPhysics::BeforeDestroy()
{
	world->release();
	world = nullptr;
	foundation->release();
	foundation = nullptr;
	Super::BeforeDestroy();
}

void SPhysics::UploadBodies()
{
	ScopedGameZone("Upload Bodies");

	auto view = ECS()->View<CTransform, CBody>();

	// Upload valid bodies to physics (Splitted between all threads)
	physicsMTFlow.parallel_for(view.begin(), view.end(), [&view](EntityId entity)
	{
		ScopedGameZone("Upload Body");

		const CBody& body = view.get<CBody>(entity);
		if (body.IsInitialized() && !body.IsStatic())
		{
			const CTransform& transform = view.get<CTransform>(entity);

			const v3& location = transform.GetWLocation();
			const Quat& rotation = transform.GetWRotation();
			const auto currTransform = body.rigidBody->getGlobalPose();

			// If position or rotation changed, upload it
			if (location.DistanceSqrt(FromPx(currTransform.p)) > Math::SMALL_NUMBER ||
				!rotation.Equals(FromPx(currTransform.q)))
			{
				const physx::PxTransform newTransform{ ToPx(location), ToPx(rotation) };
				body.rigidBody->setGlobalPose(newTransform);
			}
		}
	});
	physicsMTFlow.wait_for_all();

	// Initialize invalid bodies
	for (EntityId entity : view)
	{
		CBody& body = view.get<CBody>(entity);
		if (!body.IsInitialized())
		{
			CreateBody(entity, view.get<CTransform>(entity), body);
		}
	}
}

void SPhysics::Step(float deltaTime)
{
	ScopedGameZone("Step");

	scene->simulate(deltaTime);
	// #TODO: Support multi-threading while doing Render tick
	scene->fetchResults(true);
}

void SPhysics::DownloadBodies()
{
	ScopedGameZone("Download Bodies");

	auto view = ECS()->View<CTransform, CBody>();

	// Download valid bodies from physics (Splitted between all threads)
	physicsMTFlow.parallel_for(view.begin(), view.end(), [&view](EntityId entity)
	{
		ScopedGameZone("Download Body");

		const CBody& body = view.get<CBody>(entity);
		if (body.IsInitialized() && !body.IsStatic())
		{
			CTransform& transform = view.get<CTransform>(entity);

			transform.SetWLocation(FromPx(body.rigidBody->getGlobalPose().p));
			transform.SetWRotation(FromPx(body.rigidBody->getGlobalPose().q));
		}
	});
	physicsMTFlow.wait_for_all();
}

void SPhysics::CreateScene()
{
	physx::PxSceneDesc sceneDesc(world->getTolerancesScale());
	sceneDesc.gravity = ToPx(physicsWorld->gravity);

	if (!sceneDesc.cpuDispatcher)
	{
		// Number of desired threads
		sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		if (!sceneDesc.cpuDispatcher)
		{
			Log::Error("PhysX error: PxDefaultCpuDispatcherCreate failed!");
		}
	}

	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

	//sceneDesc.frictionType = PxFrictionType::eTWO_DIRECTIONAL;
	//sceneDesc.frictionType = PxFrictionType::eONE_DIRECTIONAL;
	//sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;
	//sceneDesc.flags |= PxSceneFlag::eENABLE_AVERAGE_POINT;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;
	//sceneDesc.flags |= PxSceneFlag::eADAPTIVE_FORCE;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;
	sceneDesc.sceneQueryUpdateMode = physx::PxSceneQueryUpdateMode::eBUILD_ENABLED_COMMIT_DISABLED;

	//sceneDesc.flags |= PxSceneFlag::eDISABLE_CONTACT_CACHE;
	//sceneDesc.broadPhaseType =  PxBroadPhaseType::eGPU;
	//sceneDesc.broadPhaseType = PxBroadPhaseType::eSAP;
	sceneDesc.gpuMaxNumPartitions = 8;

	//sceneDesc.solverType = PxSolverType::eTGS;

#ifdef USE_MBP
	sceneDesc.broadPhaseType = PxBroadPhaseType::eMBP;
#endif

	sceneDesc.simulationEventCallback = &simulationCallback;
	sceneDesc.filterShader = CollisionFilterShader;

	scene = world->createScene(sceneDesc);
	if (!scene)
	{
		Log::Error("PhysX error: CreateScene failed!");
	}
}

void SPhysics::CreateBody(EntityId entity, const CTransform& transform, CBody& body)
{
	physx::PxTransform t{ ToPx(transform.GetWLocation()), ToPx(transform.GetWRotation()) };

	switch (EMobilityType(body.mobility))
	{
	case EMobilityType::Movable:
		body.rigidBody = world->createRigidDynamic(t);
		break;
	case EMobilityType::Kinematic: {
		physx::PxRigidBody* rigidBody = world->createRigidDynamic(t);
		rigidBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
		body.rigidBody = rigidBody;
		break;
	}
	case EMobilityType::Static:
		body.rigidBody = world->createRigidStatic(t);
		break;
	}

	SetupBodyShapes(body);
	SetActorEntity(body.rigidBody, entity);
	scene->addActor(*body.rigidBody);
}

void SPhysics::SetupBodyShapes(CBody& body)
{
	physx::PxMaterial* material = world->createMaterial(0.5f, 0.5f, 0.1f);

	if (body.bHasBoxShape)
	{
		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(
			*body.rigidBody,
			physx::PxBoxGeometry(ToPx(body.boxExtent)),
			*material
		);

		if (body.bIsTrigger)
		{
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}
	}

	if (body.bHasSphereShape)
	{
		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(*body.rigidBody, physx::PxSphereGeometry(body.radius), *material);

		if (body.bIsTrigger)
		{
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}
	}
}
